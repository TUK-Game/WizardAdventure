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
		if ((int)ECollision_Channel::Wall == i)
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

			if (!MeshBoxCheck(triangles, dest->GetBoundingBox()))
			{
				std::cout << "충돌아님\n";
				continue;
			}

			src->CallCollisionBegin(dest);
			dest->CallCollisionBegin(src);
			return true;
		}
	}
	return false;
}

bool CLevelCollision::MeshBoxCheck(const std::vector<WorldTriangle>& triangles, const BoundingBox& box)
{
	for (const WorldTriangle& tri : triangles)
	{
		if (IntersectTriangleWithBoundingBox(tri, box)) 
			return true; 
	}

	return false; 
}

bool CLevelCollision::IntersectTriangleWithBoundingBox(const WorldTriangle& tri, const BoundingBox& box)
{
	Vec3 v0 = tri.a - box.Center;
	Vec3 v1 = tri.b - box.Center;
	Vec3 v2 = tri.c - box.Center;

	Vec3 f0 = v1 - v0;
	Vec3 f1 = v2 - v1;
	Vec3 f2 = v0 - v2;

	const Vec3 axes[9] = {
		Vec3(0, -f0.z, f0.y), Vec3(0, -f1.z, f1.y), Vec3(0, -f2.z, f2.y),
		Vec3(f0.z, 0, -f0.x), Vec3(f1.z, 0, -f1.x), Vec3(f2.z, 0, -f2.x),
		Vec3(-f0.y, f0.x, 0), Vec3(-f1.y, f1.x, 0), Vec3(-f2.y, f2.x, 0)
	};

	for (int i = 0; i < 9; ++i)
	{
		const Vec3& axis = axes[i];
		if (axis.LengthSquared() < 1e-6f) continue;

		float minT, maxT;
		float r = box.Extents.x * fabsf(axis.x) + box.Extents.y * fabsf(axis.y) + box.Extents.z * fabsf(axis.z);
		ProjectTriangle(axis, v0, v1, v2, minT, maxT);

		if (minT > r || maxT < -r)
			return false;
	}

	{
		float minT = (std::min)({ v0.x, v1.x, v2.x });
		float maxT = (std::max)({ v0.x, v1.x, v2.x });
		if (minT > box.Extents.x || maxT < -box.Extents.x)
			return false;
	}

	{
		float minT = (std::min)({ v0.y, v1.y, v2.y });
		float maxT = (std::max)({ v0.y, v1.y, v2.y });
		if (minT > box.Extents.y || maxT < -box.Extents.y)
			return false;
	}

	{
		float minT = (std::min)({ v0.z, v1.z, v2.z });
		float maxT = (std::max)({ v0.z, v1.z, v2.z });
		if (minT > box.Extents.z || maxT < -box.Extents.z)
			return false;
	}


	// 삼각형 평면 법선
	Vec3 normal = (v1 - v0).Cross(v2 - v0);
	float minT, maxT;
	float r = box.Extents.x * fabsf(normal.x) + box.Extents.y * fabsf(normal.y) + box.Extents.z * fabsf(normal.z);
	ProjectTriangle(normal, v0, v1, v2, minT, maxT);

	if (minT > r || maxT < -r)
		return false;

	return true;
}

void CLevelCollision::ProjectTriangle(const Vec3& axis, const Vec3& v0, const Vec3& v1, const Vec3& v2, float& outMin, float& outMax)
{
	float p0 = axis.Dot(v0);
	float p1 = axis.Dot(v1);
	float p2 = axis.Dot(v2);

	outMin = (std::min)({ p0, p1, p2 });
	outMax = (std::max)({ p0, p1, p2 });
}
