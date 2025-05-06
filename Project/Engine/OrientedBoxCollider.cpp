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

bool COrientedBoxCollider::Intersects(const Vec4& rayOrigin, const Vec4& rayDir, OUT float& distance)
{
	return m_BoundingOrientedBox.Intersects(rayOrigin, rayDir, OUT distance);
}

bool COrientedBoxCollider::IsFrustum(CFrustum& frustum)
{
	if (!frustum.IsInFrustum(m_BoundingOrientedBox))
		return false;
	return true;
}

void COrientedBoxCollider::SetMaxMinPos(Vec4 centerPos, Vec3 maxPos, Vec3 minPos, Vec3 offset)
{
	Vec3 pos = GetTransform()->GetRelativePosition();
	center = Vec3(pos.x, pos.y, pos.z);
	m_BoundingOrientedBox.Center = Vec3(pos.x, pos.y, pos.z);
	m_BoundingOrientedBox.Extents = ((maxPos - minPos)) / 2;
	size = (maxPos - minPos) / 2;
	Matrix rotationMatrix = GetOwner()->GetTransform()->GetWorldRotationMatrix();
	XMStoreFloat4(&m_BoundingOrientedBox.Orientation, XMQuaternionRotationMatrix(rotationMatrix));
}

void COrientedBoxCollider::SetMaxMinPos(Vec3 centerPos, Vec3 maxPos, Vec3 minPos, Vec3 offset)
{
	center = Vec3(centerPos.x, centerPos.y, centerPos.z);
	m_BoundingOrientedBox.Center = Vec3(centerPos.x, centerPos.y, centerPos.z);
	m_BoundingOrientedBox.Extents = (maxPos - minPos) / 2;
	m_Offset = offset;
	Matrix rotationMatrix = GetOwner()->GetTransform()->GetWorldRotationMatrix();
	XMStoreFloat4(&m_BoundingOrientedBox.Orientation, XMQuaternionRotationMatrix(rotationMatrix));
	size = (maxPos - minPos) / 2;
}

void COrientedBoxCollider::FinalUpdate()
{
	if (GetProfile()->channel == ECollision_Channel::Wall)
		return;

	Vec3 scale = GetOwner()->GetTransform()->GetRelativeScale();
	m_BoundingOrientedBox.Center = GetOwner()->GetTransform()->GetWorldPosition();
	//m_BoundingOrientedBox.Extents = XMFLOAT3(scale.x * 0.5f, scale.y * 0.5f, scale.z * 0.5f);
	Matrix rotationMatrix = GetOwner()->GetTransform()->GetWorldRotationMatrix();
	XMStoreFloat4(&m_BoundingOrientedBox.Orientation, XMQuaternionRotationMatrix(rotationMatrix));
}

