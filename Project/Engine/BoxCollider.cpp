#include "pch.h"
#include "BoxCollider.h"
#include "Transform.h"
#include "Frustum.h"
#include "CollisionManager.h"
#include "LevelManager.h"
#include "Level.h"
#include "LevelCollision.h"
#include "MeshRenderer.h"

CBoxCollider::CBoxCollider()
	: CBaseCollider(ECollider_Type::Box)
{
}

CBoxCollider::~CBoxCollider()
{
}

bool CBoxCollider::Intersects(const Vec4& rayOrigin, const Vec4& rayDir, OUT float& distance)
{
	return m_BoundingBox.Intersects(rayOrigin, rayDir, OUT distance); 
}

bool CBoxCollider::IsFrustum(CFrustum& frustum)
{
	if (!frustum.IsInFrustum(m_BoundingBox))
	{
		m_IsFrustum = true;
		return false;
	}
	else
	{
		m_IsFrustum = false;
		return true;
	}
}

bool CBoxCollider::Collision(CBaseCollider* dest)
{
	switch (dest->GetColliderType())
	{
	case ECollider_Type::OrientedBox:
		//return CCollisionManager::GetInst()->CollisionBoxToBox(m_hitPoint, this, (CColliderBox*)dest);
		break;
	case ECollider_Type::Box:
		return CCollisionManager::GetInst()->CollisionBoxToBox(this, (CBoxCollider*)dest);
	}

	return false;
}

void CBoxCollider::SetMaxMinPos(Vec4 centerPos, Vec3 maxPos, Vec3 minPos, Vec3 offset)
{
	Vec3 pos = GetTransform()->GetRelativePosition();
	center = Vec3(pos.x, pos.y, pos.z);
	m_BoundingBox.Center = Vec3(pos.x, pos.y, pos.z);
	m_BoundingBox.Extents = ((maxPos - minPos)) / 2;
	size = (maxPos - minPos) / 2;
}

void CBoxCollider::SetMaxMinPos(Vec3 centerPos, Vec3 maxPos, Vec3 minPos, Vec3 offset)
{
	center = Vec3(centerPos.x, centerPos.y, centerPos.z);
	m_BoundingBox.Center = Vec3(centerPos.x, centerPos.y, centerPos.z);
	m_BoundingBox.Extents = (maxPos - minPos) / 2;
	m_Offset = offset;
	size = (maxPos - minPos) / 2;

}

void CBoxCollider::FinalUpdate()	
{
	if (GetProfile()->channel == ECollision_Channel::Wall)
	{
		if(!m_IsFrustum)
			CLevelManager::GetInst()->GetCurrentLevel()->GetLevelCollision()->AddCollider(this);
		return;
	}

	m_BoundingBox.Center = GetOwner()->GetTransform()->GetWorldPosition() + m_Offset;

	//Vec3 scale = GetOwner()->GetMeshRenderer()->GetMesh()->GetMeshSize() * (GetOwner()->GetTransform()->GetRelativeScale());
	//m_BoundingBox.Extents = XMFLOAT3(scale.x, scale.y, scale.z);

	CLevelManager::GetInst()->GetCurrentLevel()->GetLevelCollision()->AddCollider(this);
}
