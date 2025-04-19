#include "pch.h"
#include "CreatureCollider.h"
#include "GameObject.h"
#include "Room.h"
#include "LevelCollision.h"
#include "Player.h"

void CCreatureCollider::Update()
{
	Protocol::Vector3 pos = ((CPlayer*)m_Owner)->PlayerInfo->object_info().pos_info().position();
	Protocol::Vector3 rot = ((CPlayer*)m_Owner)->PlayerInfo->object_info().pos_info().rotation();
	m_BoundingBox.Center = XMFLOAT3(pos.x() + m_Offset.x, pos.y() + m_Offset.y, pos.z() + m_Offset.z);
	XMVECTOR rotRad = XMVectorSet(
		XMConvertToRadians(rot.x()),
		XMConvertToRadians(rot.y()),
		XMConvertToRadians(rot.z()),
		0.0f
	);

	XMVECTOR quatX = XMQuaternionRotationAxis(XMVectorSet(1, 0, 0, 0), XMVectorGetX(rotRad));
	XMVECTOR quatY = XMQuaternionRotationAxis(XMVectorSet(0, 1, 0, 0), XMVectorGetY(rotRad));
	XMVECTOR quatZ = XMQuaternionRotationAxis(XMVectorSet(0, 0, 1, 0), XMVectorGetZ(rotRad));

	XMVECTOR finalQuat = XMQuaternionMultiply(quatX, XMQuaternionMultiply(quatY, quatZ));

	XMStoreFloat4(&m_BoundingBox.Orientation, finalQuat);
	// temp
	g_Room->GetLevelCollision()->AddCollider(this, ECollision_Channel::Player);
}

void CCreatureCollider::SetBoxInfo(XMFLOAT3 centerPos, XMFLOAT3 size, XMFLOAT3 rotation, XMFLOAT3 offset)
{
	m_BoundingBox.Center = centerPos;
	m_BoundingBox.Extents = XMFLOAT3(size.x / 2, size.y / 2, size.z / 2);
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
	((CPlayer*)m_Owner)->PlayerInfo->mutable_object_info()->mutable_pos_info()->mutable_position()->set_x(centerPos.x);
	((CPlayer*)m_Owner)->PlayerInfo->mutable_object_info()->mutable_pos_info()->mutable_position()->set_y(centerPos.y);
	((CPlayer*)m_Owner)->PlayerInfo->mutable_object_info()->mutable_pos_info()->mutable_position()->set_z(centerPos.z);
}
