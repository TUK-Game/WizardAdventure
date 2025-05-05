#include "pch.h"
#include "ProjectileCollider.h"
#include "Room.h"
#include "Projectile.h"
#include "LevelCollision.h"

void CProjectileCollider::Update()
{
	const auto& pos = ((CProjectile*)m_Owner)->ProjectileInfo->object_info().pos_info().position();
	const auto& rotation = ((CProjectile*)m_Owner)->ProjectileInfo->object_info().pos_info().rotation();
	//const auto& size = ((CProjectile*)m_Owner)->ProjectileInfo->object_info().pos_info().size();
    Vec3 ms = ((CProjectile*)m_Owner)->GetMeshSize();
	m_BoundingBox.Center = Vec3(pos.x() + m_Offset.x, pos.y() + m_Offset.y, pos.z() + m_Offset.z);
    //m_BoundingBox.Extents = XMFLOAT3(size.x() * ms.x, size.y() * ms.y, size.z() * ms.z);

    XMVECTOR rotRad = XMVectorSet(
        XMConvertToRadians(rotation.x()),
        XMConvertToRadians(rotation.y()),
        XMConvertToRadians(rotation.z()),
        0.0f
    );

    XMVECTOR quatX = XMQuaternionRotationAxis(XMVectorSet(1, 0, 0, 0), XMVectorGetX(rotRad));
    XMVECTOR quatY = XMQuaternionRotationAxis(XMVectorSet(0, 1, 0, 0), XMVectorGetY(rotRad));
    XMVECTOR quatZ = XMQuaternionRotationAxis(XMVectorSet(0, 0, 1, 0), XMVectorGetZ(rotRad));

    XMVECTOR q = XMQuaternionMultiply(quatX, XMQuaternionMultiply(quatY, quatZ));

    XMStoreFloat4(&m_BoundingBox.Orientation, q);

	g_Room->GetLevelCollision()->AddCollider(this, m_Channel);
}

void CProjectileCollider::SetBoxInfo(const Vec3& centerPos, const Vec3& size, const Vec3& rotation, const Vec3& offset)
{
    m_BoundingBox.Center = centerPos;

    m_BoundingBox.Extents = Vec3(size.x / 2 , size.y / 2, size.z / 2);

    XMVECTOR rotRad = XMVectorSet(
        XMConvertToRadians(rotation.x),
        XMConvertToRadians(rotation.y),
        XMConvertToRadians(rotation.z),
        0.0f
    );

    XMVECTOR quatX = XMQuaternionRotationAxis(XMVectorSet(1, 0, 0, 0), XMVectorGetX(rotRad));
    XMVECTOR quatY = XMQuaternionRotationAxis(XMVectorSet(0, 1, 0, 0), XMVectorGetY(rotRad));
    XMVECTOR quatZ = XMQuaternionRotationAxis(XMVectorSet(0, 0, 1, 0), XMVectorGetZ(rotRad));

    XMVECTOR q = XMQuaternionMultiply(quatX, XMQuaternionMultiply(quatY, quatZ));

    XMStoreFloat4(&m_BoundingBox.Orientation, q);

    m_Offset = offset;
    ((CProjectile*)m_Owner)->ProjectileInfo->mutable_object_info()->mutable_pos_info()->mutable_position()->set_x(centerPos.x);
    ((CProjectile*)m_Owner)->ProjectileInfo->mutable_object_info()->mutable_pos_info()->mutable_position()->set_y(centerPos.y);
    ((CProjectile*)m_Owner)->ProjectileInfo->mutable_object_info()->mutable_pos_info()->mutable_position()->set_z(centerPos.z);
}
