#pragma once

class CCollisionManager
	: public CSingleton<CCollisionManager>
{
	DECLARE_SINGLETON(CCollisionManager)

public:
	bool Init();

	bool CreateProfile(const std::string& name, ECollision_Channel channel, bool enable, ECollision_Interaction baseInteraction = ECollision_Interaction::Collision);
	bool SetCollisionInteraction(std::string_view name, ECollision_Channel channel, ECollision_Interaction interaction);
	CollisionProfile* FindProfile(std::string_view name);

public:
	bool CollisionBoxToBox(class CBoxCollider* src, class CBoxCollider* dest);
	//bool CollisionBoxToBox(Vector2& hitPoint, class CColliderBox* src, class CColliderBox* dest);
	//bool CollisionCircleToCircle(Vector2& hitPoint, class CColliderCircle* src, class CColliderCircle* dest);
	//bool CollisionBoxToCircle(Vector2& hitPoint, class CColliderBox* src, class CColliderCircle* dest);
	//bool CollisionPointToBox(Vector2& hitPoint, const Vector2& src, class CColliderBox* dest);
	//bool CollisionPointToCircle(Vector2& hitPoint, const Vector2& src, class CColliderCircle* dest);

	//bool CollisionBoxToBox(Vector2& hitPoint, const BoxInfo& src, const BoxInfo& dest);
	//bool CollisionCircleToCircle(Vector2& hitPoint, const CircleInfo& src, const CircleInfo& dest);
	//bool CollisionBoxToCircle(Vector2& hitPoint, const BoxInfo& src, const CircleInfo& dest);
	//bool CollisionPointToBox(Vector2& hitPoint, const Vector2& src, const BoxInfo& dest);
	//bool CollisionPointToCircle(Vector2& hitPoint, const Vector2& src, const CircleInfo& dest);

private:
	std::unordered_map<std::string, CollisionProfile*>	m_mapProfile;
};

