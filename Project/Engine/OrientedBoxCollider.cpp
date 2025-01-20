#include "pch.h"
#include "OrientedBoxCollider.h"
#include "Transform.h"
#include "Frustum.h"

COrientedBoxCollider::COrientedBoxCollider()
	: CBaseCollider(ECollider_Type::Box)
{
}

COrientedBoxCollider::~COrientedBoxCollider()
{
}

bool COrientedBoxCollider::Intersects(Vec4 rayOrigin, Vec4 rayDir, OUT float& distance)
{
	return m_BoundingOrientedBox.Intersects(rayOrigin, rayDir, OUT distance);
}

bool COrientedBoxCollider::IsFrustum(CFrustum frustum)
{
	if (!frustum.IsInFrustum(m_BoundingOrientedBox))
		return false;
	return true;
}

void COrientedBoxCollider::FinalUpdate()
{
	Vec3 scale = GetOwner()->GetTransform()->GetRelativeScale();
	m_BoundingOrientedBox.Center = GetOwner()->GetTransform()->GetWorldPosition();
	m_BoundingOrientedBox.Extents = XMFLOAT3(scale.x * 0.5f, scale.y * 0.5f, scale.z * 0.5f);
	Matrix rotationMatrix = GetOwner()->GetTransform()->GetWorldRotationMatrix();
	XMStoreFloat4(&m_BoundingOrientedBox.Orientation, XMQuaternionRotationMatrix(rotationMatrix));
}

