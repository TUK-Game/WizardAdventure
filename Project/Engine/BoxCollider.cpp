#include "pch.h"
#include "BoxCollider.h"
#include "Transform.h"
#include "Frustum.h"

CBoxCollider::CBoxCollider()
	: CBaseCollider(EColliderType::Box)
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

void CBoxCollider::FinalUpdate()
{
	m_BoundingBox.Center = GetOwner()->GetTransform()->GetWorldPosition();

	Vec3 scale = GetOwner()->GetTransform()->GetRelativeScale();
	m_BoundingBox.Extents = XMFLOAT3(scale.x * 0.5f, scale.y * 0.5f, scale.z * 0.5f);
}
