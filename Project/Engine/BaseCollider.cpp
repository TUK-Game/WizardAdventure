#include "pch.h"
#include "BaseCollider.h"
#include "Transform.h"
#include "CollisionManager.h"

CBaseCollider::CBaseCollider(ECollider_Type colliderType)
	: CComponent(EComponent_Type::Collider), m_ColliderType(colliderType)
{
}

CBaseCollider::~CBaseCollider()
{
}

void CBaseCollider::CreateCollisionProfile(std::string name, ECollision_Channel channel)
{
	m_profile = new CollisionProfile();
	m_profile->name = name;
	m_profile->channel = channel;
	m_profile->vecCollisionInteraction.resize((int)(ECollision_Channel::Max));
}

void CBaseCollider::SetCollisionProfile(const std::string& name)
{
	m_profile = CCollisionManager::GetInst()->FindProfile(name);
}

void CBaseCollider::AddCollisionList(CBaseCollider* collider)
{
	m_collisionList.push_back(collider);
}

bool CBaseCollider::CheckCollisionList(CBaseCollider* collider)
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

void CBaseCollider::DeleteCollisionList(CBaseCollider* collider)
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

void CBaseCollider::ClearCollisionList()
{
	auto	iter = m_collisionList.begin();
	auto	iterEnd = m_collisionList.end();
	for (; iter != iterEnd; ++iter)
	{
		(*iter)->DeleteCollisionList(this);
	}

	m_collisionList.clear();
}

void CBaseCollider::CallCollisionBegin(CBaseCollider* dest)
{
	GetOwner()->CollisionBegin(this, dest);
}

void CBaseCollider::CallCollisionEnd(CBaseCollider* dest)
{
	GetOwner()->CollisionEnd(this, dest);
}