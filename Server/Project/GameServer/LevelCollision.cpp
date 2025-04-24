#include "pch.h"
#include "LevelCollision.h"
#include "BoxCollider.h"
#include "GameObject.h"

CLevelCollision::CLevelCollision()
{
}

CLevelCollision::~CLevelCollision()
{
}

void CLevelCollision::AddCollider(CBoxCollider* collider, ECollision_Channel channel)
{
	m_vecCollider[(int)channel].push_back(collider);
}

void CLevelCollision::Collision()
{
	for (int i = 0; i < (int)ECollision_Channel::Max; ++i)
	{
		size_t srcSize = m_vecCollider[i].size();
		if (srcSize == 0 || (int)ECollision_Channel::Wall == i)
			continue;

		for (int j = 0; j < (int)ECollision_Channel::Max; ++j)
		{
			size_t destSize = m_vecCollider[j].size();
			if (destSize == 0 || (int)ECollision_Channel::Wall == j)
				continue;

			for (size_t s = 0; s < srcSize; ++s)
			{
				CBoxCollider* src = m_vecCollider[i][s];
				const CollisionProfile* srcProfile = src->GetProfile();

				size_t start = (i == j) ? s + 1 : 0;
				for (size_t d = start; d < destSize; ++d)
				{
					CBoxCollider* dest = m_vecCollider[j][d];
					if (src == dest || src->GetOwner() == dest->GetOwner())
						continue;

					const CollisionProfile* destProfile = dest->GetProfile();

					ECollision_Interaction srcInteraction = srcProfile->vecCollisionInteraction[(int)destProfile->channel];
					ECollision_Interaction destInteraction = destProfile->vecCollisionInteraction[(int)srcProfile->channel];

					if (srcInteraction == ECollision_Interaction::Ignore || destInteraction == ECollision_Interaction::Ignore)
						continue;

					if (src->Collision(dest))
					{
						if (!src->CheckCollisionList(dest))
						{
							src->AddCollisionList(dest);
							dest->AddCollisionList(src);

							src->CallCollisionBegin(dest);
							dest->CallCollisionBegin(src);
						}
						else
						{
							src->CollisionEvent(dest);
							dest->CollisionEvent(src);
						}
					}
					else if (src->CheckCollisionList(dest))
					{
						src->DeleteCollisionList(dest);
						dest->DeleteCollisionList(src);

						src->CallCollisionEnd(dest);
						dest->CallCollisionEnd(src);
					}
				}
			}
		}
	}

	for (int i = 0; i < (int)ECollision_Channel::Max; ++i)
	{
		if ((int)ECollision_Channel::Wall == i || (int)ECollision_Channel::Player == i)
			continue;

		m_vecCollider[i].clear();
	}
}

bool CLevelCollision::CollisionWithWall(CBoxCollider* collider)
{
	size_t size = m_vecCollider[(int)ECollision_Channel::Wall].size();
	if (size > 1)
	{
		CBoxCollider* dest = collider;
		for (size_t i = 0; i < size; ++i)
		{
			CBoxCollider* src = m_vecCollider[(int)ECollision_Channel::Wall][i];

			// 충돌 체크
			if (!src->Collision(dest))
				continue;

			const std::vector<WorldTriangle>& triangles = src->GetWorldTriangle();

			if (!triangles.empty())
			{
				if (!MeshBoxCheck(triangles, dest->GetBoundingBox()))
					continue;
			}

			src->CallCollisionBegin(dest);
			dest->CallCollisionBegin(src);
			return true;
		}
	}
	return false;
}

bool CLevelCollision::CollisionWithPlayer(CBoxCollider* collider)
{
	size_t size = m_vecCollider[(int)ECollision_Channel::Player].size();
	if (size > 1)
	{
		CBoxCollider* dest = collider;
		for (size_t i = 0; i < size; ++i)
		{
			CBoxCollider* src = m_vecCollider[(int)ECollision_Channel::Player][i];

			if (dest == src)
				continue;

			// 충돌 체크
			if (src->Collision(dest))
				return true;
		}
	}
	return false;
}

bool CLevelCollision::MeshBoxCheck(const std::vector<WorldTriangle>& triangles, const BoundingOrientedBox& box)
{
	for (const WorldTriangle& tri : triangles)
	{
		if (IntersectTriangleWithBoundingBox(tri, box)) 
			return true; 
	}

	return false; 
}

bool CLevelCollision::IntersectTriangleWithBoundingBox(const WorldTriangle& tri, const BoundingOrientedBox& obb)
{
	// 1. 삼각형 정점 가져오기 (월드 공간)
	Vec3 v0 = tri.a;
	Vec3 v1 = tri.b;
	Vec3 v2 = tri.c;

	// 2. OBB의 로컬 축 3개 추출 (Orientation 반영)
	XMVECTOR quat = XMLoadFloat4(&obb.Orientation);

	XMVECTOR right = XMVector3Rotate(XMVectorSet(1, 0, 0, 0), quat);
	XMVECTOR up = XMVector3Rotate(XMVectorSet(0, 1, 0, 0), quat);
	XMVECTOR forward = XMVector3Rotate(XMVectorSet(0, 0, 1, 0), quat);

	Vec3 u = Vec3(right);
	Vec3 v = Vec3(up);
	Vec3 w = Vec3(forward);

	// 3. 삼각형을 OBB 중심 기준으로 이동
	Vec3 center(obb.Center);
	Vec3 p0 = v0 - center;
	Vec3 p1 = v1 - center;
	Vec3 p2 = v2 - center;

	// 4. 삼각형 Edge
	Vec3 e0 = p1 - p0;
	Vec3 e1 = p2 - p1;
	Vec3 e2 = p0 - p2;

	// 5. 검사할 축 목록
	std::vector<Vec3> axes;

	// OBB의 축 3개
	axes.push_back(u);
	axes.push_back(v);
	axes.push_back(w);

	// 삼각형 법선
	Vec3 triNormal = (p1 - p0).Cross(p2 - p0);
	axes.push_back(triNormal);

	// edge x axis (총 9개)
	for (Vec3 edge : { e0, e1, e2 })
	{
		for (Vec3 axis : { u, v, w })
		{
			Vec3 cross = edge.Cross(axis);
			if (cross.LengthSquared() > 1e-6f)
			{
				cross.Normalize(); // 자기 자신을 정규화
				axes.push_back(cross);
			}
		}
	}

	// 6. SAT 검사
	for (const Vec3& axis : axes)
	{
		float triMin, triMax;
		float boxRadius =
			obb.Extents.x * fabsf(axis.Dot(u)) +
			obb.Extents.y * fabsf(axis.Dot(v)) +
			obb.Extents.z * fabsf(axis.Dot(w));

		ProjectTriangle(axis, p0, p1, p2, triMin, triMax);

		if (triMin > boxRadius || triMax < -boxRadius)
			return false; // 분리축 발견
	}

	return true; // 충돌
}
void CLevelCollision::ProjectTriangle(const Vec3& axis, const Vec3& v0, const Vec3& v1, const Vec3& v2, float& outMin, float& outMax)
{
	float p0 = axis.Dot(v0);
	float p1 = axis.Dot(v1);
	float p2 = axis.Dot(v2);

	outMin = (std::min)({ p0, p1, p2 });
	outMax = (std::max)({ p0, p1, p2 });
}
