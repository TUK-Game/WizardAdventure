#include "pch.h"
#include "CollisionManager.h"
#include "BoxCollider.h"

CCollisionManagerRef g_CollisionManager = std::make_shared<CCollisionManager>();

CCollisionManager::CCollisionManager()
{
}

CCollisionManager::~CCollisionManager()
{
	for (auto& [key, value] : m_mapProfile)
	{
		if (value)	
		{
			delete value;
			value = nullptr;
		}
	}
	m_mapProfile.clear();
}

void CCollisionManager::Init()
{
	CreateProfile("Wall", ECollision_Channel::Wall, true, ECollision_Interaction::Ignore);
	CreateProfile("Player", ECollision_Channel::Player, true);
	CreateProfile("Monster", ECollision_Channel::Monster, true);
	CreateProfile("Projectile", ECollision_Channel::Monster, true);

	SetCollisionInteraction("Projectile", ECollision_Channel::Player, ECollision_Interaction::Ignore);
	SetCollisionInteraction("Projectile", ECollision_Channel::Wall, ECollision_Interaction::Collision);

	SetCollisionInteraction("Monster", ECollision_Channel::Wall, ECollision_Interaction::Collision);

	SetCollisionInteraction("Wall", ECollision_Channel::Wall, ECollision_Interaction::Ignore);
	SetCollisionInteraction("Wall", ECollision_Channel::Player, ECollision_Interaction::Collision);
}

bool CCollisionManager::CreateProfile(const std::string& name, ECollision_Channel channel, bool enable, ECollision_Interaction baseInteraction)
{
	if (FindProfile(name))
		return false;

	CollisionProfile* profile = new CollisionProfile;
	profile->name = name;
	profile->channel = channel;
	profile->enable = enable;
	profile->vecCollisionInteraction.resize((size_t)ECollision_Channel::Max);

	for (auto& interaction : profile->vecCollisionInteraction)
	{
		interaction = baseInteraction;
	}

	m_mapProfile.insert(std::make_pair(name, profile));

	return true;
}

bool CCollisionManager::SetCollisionInteraction(std::string_view name, ECollision_Channel channel, ECollision_Interaction interaction)
{
	CollisionProfile* profile = FindProfile(name);

	if (!profile)
		return false;

	profile->vecCollisionInteraction[(int)channel] = interaction;

	return true;
}

CollisionProfile* CCollisionManager::FindProfile(std::string_view name)
{
	auto iter = m_mapProfile.find(name.data());

	if (iter == m_mapProfile.end())
		return nullptr;

	return iter->second;
}

bool CCollisionManager::CollisionBoxToBox(CBoxCollider* src, CBoxCollider* dest)
{
	return src->GetBoundingBox().Intersects(dest->GetBoundingBox());
}
