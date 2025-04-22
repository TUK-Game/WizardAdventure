#include "pch.h"
#include "BoxCollider.h"
#include "GameObject.h"
#include "CollisionManager.h"
#include "Room.h"
#include "LevelCollision.h"

CBoxCollider::CBoxCollider()
{
}

CBoxCollider::~CBoxCollider()
{
}

bool CBoxCollider::Collision(CBoxCollider* dest)
{
	return g_CollisionManager->CollisionBoxToBox(this, (CBoxCollider*)dest);
}

void CBoxCollider::CreateCollisionProfile(std::string name, ECollision_Channel channel)
{
	m_profile = new CollisionProfile();
	m_profile->name = name;
	m_profile->channel = channel;
	m_profile->vecCollisionInteraction.resize((int)(ECollision_Channel::Max));
}

void CBoxCollider::SetCollisionProfile(const std::string& name)
{
	m_profile = g_CollisionManager->FindProfile(name);
	m_Channel = m_profile->channel;
}

void CBoxCollider::AddCollisionList(CBoxCollider* collider)
{
	m_collisionList.push_back(collider);
}

bool CBoxCollider::CheckCollisionList(CBoxCollider* collider)
{
	auto	iter = m_collisionList.begin();
	auto	iterEnd = m_collisionList.end();
	for (; iter != iterEnd; ++iter)
	{
		if (*iter == collider)
			return true;
	}

	return false;
}

void CBoxCollider::DeleteCollisionList(CBoxCollider* collider)
{
	auto	iter = m_collisionList.begin();
	auto	iterEnd = m_collisionList.end();
	for (; iter != iterEnd; ++iter)
	{
		if (*iter == collider)
		{
			m_collisionList.erase(iter);
			break;
		}
	}
}

void CBoxCollider::ClearCollisionList()
{
	auto	iter = m_collisionList.begin();
	auto	iterEnd = m_collisionList.end();
	for (; iter != iterEnd; ++iter)
	{
		(*iter)->DeleteCollisionList(this);
	}

	m_collisionList.clear();
}

void CBoxCollider::CallCollisionBegin(CBoxCollider* dest)
{
	GetOwner()->CollisionBegin(this, dest);
}

void CBoxCollider::CollisionEvent(CBoxCollider* dest)
{
	GetOwner()->CollisionEvent(this, dest);
}

void CBoxCollider::CallCollisionEnd(CBoxCollider* dest)
{
	GetOwner()->CollisionEnd(this, dest);
}

void CBoxCollider::SetBoxInfo(const Vec3& centerPos, const Vec3& size, const Vec3& rotation, const Vec3& offset)
{
	m_BoundingBox.Center = centerPos;
	m_BoundingBox.Extents = XMFLOAT3(size.x, size.y, size.z);
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

	m_Owner->PosInfo->mutable_position()->set_x(centerPos.x);
	m_Owner->PosInfo->mutable_position()->set_y(centerPos.y);
	m_Owner->PosInfo->mutable_position()->set_z(centerPos.z);
}
void CBoxCollider::SetBoxHeight(float height)
{
	XMFLOAT3 center = m_BoundingBox.Center;
	center.y = height;
	m_BoundingBox.Center = center;
}


void CBoxCollider::Update()
{
	if (m_Channel == ECollision_Channel::Wall)
		return;

	Protocol::Vector3 pos = m_Owner->PosInfo->position();
	m_BoundingBox.Center = XMFLOAT3(pos.x() + m_Offset.x, pos.y() + m_Offset.y, pos.z() + m_Offset.z) ;
	g_Room->GetLevelCollision()->AddCollider(this, m_Channel);
}
