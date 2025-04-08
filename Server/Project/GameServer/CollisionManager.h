#pragma once
class CCollisionManager
{
public:
	CCollisionManager();
	~CCollisionManager();

public:
	void Init();

	bool CreateProfile(const std::string& name, ECollision_Channel channel, bool enable, ECollision_Interaction baseInteraction = ECollision_Interaction::Collision);
	bool SetCollisionInteraction(std::string_view name, ECollision_Channel channel, ECollision_Interaction interaction);
	CollisionProfile* FindProfile(std::string_view name);

	bool CollisionBoxToBox(class CBoxCollider* src, class CBoxCollider* dest);

private:
	std::unordered_map<std::string, CollisionProfile*>	m_mapProfile;
};

extern CCollisionManagerRef g_CollisionManager;