#include "pch.h"
#include "MonsterCollider.h"
#include "GameObject.h"
#include "Room.h"
#include "LevelCollision.h"
#include "Monster.h"

void CMonsterCollider::Update()
{
	Protocol::Vector3 pos = ((CMonster*)m_Owner)->MonsterInfo->object_info().pos_info().position();
	Protocol::Vector3 rotation = ((CMonster*)m_Owner)->MonsterInfo->object_info().pos_info().rotation();
	m_BoundingBox.Center = Vec3(pos.x() + m_Offset.x, pos.y() + m_Offset.y, pos.z() + m_Offset.z);

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
    //std::cout << m_BoundingBox.Center.x << " " << m_BoundingBox.Center.y << " " << m_BoundingBox.Center.z << '\n';
    //std::cout << m_BoundingBox.Extents.x << " " << m_BoundingBox.Extents.y << " " << m_BoundingBox.Extents.z << "\n\n";
	g_Room->GetLevelCollision()->AddCollider(this, m_Channel);
}

void CMonsterCollider::SetBoxInfo(const Vec3& centerPos, const Vec3& size, const Vec3& rotation, const Vec3& offset)
{
	m_BoundingBox.Center = centerPos;

	m_BoundingBox.Extents = Vec3(size.x / 2, size.y / 2, size.z / 2);

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
	((CMonster*)m_Owner)->MonsterInfo->mutable_object_info()->mutable_pos_info()->mutable_position()->set_x(centerPos.x);
	((CMonster*)m_Owner)->MonsterInfo->mutable_object_info()->mutable_pos_info()->mutable_position()->set_y(centerPos.y);
	((CMonster*)m_Owner)->MonsterInfo->mutable_object_info()->mutable_pos_info()->mutable_position()->set_z(centerPos.z);
}
