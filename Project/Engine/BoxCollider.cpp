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

bool CBoxCollider::Intersects(Vec4 rayOrigin, Vec4 rayDir, OUT float& distance)
{
	return m_BoundingBox.Intersects(rayOrigin, rayDir, OUT distance); 
}

bool CBoxCollider::IsFrustum(CFrustum frustum)
{
	if (!frustum.IsInFrustum(m_BoundingBox))
		return false;
	return true;
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

void CBoxCollider::SetMaxMinPos(Vec4 centerPos, Vec3 maxPos, Vec3 minPos)
{
	center = Vec3(centerPos.x, centerPos.y, -centerPos.z);
	m_BoundingBox.Center = Vec3(centerPos.x, centerPos.y, -centerPos.z);
	m_BoundingBox.Extents = (maxPos - minPos) / 2;
}

void CBoxCollider::FinalUpdate()	
{
	if (GetProfile()->channel == ECollision_Channel::Wall)
		return;

	m_BoundingBox.Center = GetOwner()->GetTransform()->GetWorldPosition();

	Vec3 scale = GetOwner()->GetMeshRenderer()->GetMesh()->GetMeshSize() * (GetOwner()->GetTransform()->GetRelativeScale());
	m_BoundingBox.Extents = XMFLOAT3(scale.x, scale.y, scale.z);

	CLevelManager::GetInst()->GetCurrentLevel()->GetLevelCollision()->AddCollider(this);
}
