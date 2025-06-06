#pragma once
#include "Component.h"

enum class ECollider_Type
{
	Box,
	OrientedBox,
	Sphere,
};

class CBaseCollider : public CComponent
{
public:
	CBaseCollider(ECollider_Type colliderType);
	virtual ~CBaseCollider();

public:
	ECollider_Type GetColliderType() { return m_ColliderType; }
	const CollisionProfile* GetProfile() const { return m_profile; }
	
	void SetOffset(Vec3 offset) { m_Offset = offset; }
	void SetProfile(CollisionProfile* profile) { m_profile = profile; }
	void CreateCollisionProfile(std::string name, ECollision_Channel channel);

public:
	virtual bool Intersects(const Vec4& rayOrigin, const Vec4& rayDir, OUT float& distance) = 0;
	virtual void SetMaxMinPos(Vec4 centerPos, Vec3 maxPos, Vec3 minPos, Vec3 offset = Vec3(0.f, 0.f, 0.f)) {}
	virtual void SetMaxMinPos(Vec3 centerPos, Vec3 maxPos, Vec3 minPos, Vec3 offset = Vec3(0.f, 0.f, 0.f)) {}
	virtual bool IsFrustum(class CFrustum& frustum) { return false; }
	virtual bool Collision(CBaseCollider* dest) { return false; }

	void SetCollisionProfile(const std::string& name);
	void AddCollisionList(CBaseCollider* collider);
	bool CheckCollisionList(CBaseCollider* collider);
	void DeleteCollisionList(CBaseCollider* collider);
	void ClearCollisionList();
	void CallCollisionBegin(CBaseCollider* dest);
	void CallCollisionEnd(CBaseCollider* dest);
	Vec3 center;
	Vec3 size;

protected:
	ECollider_Type m_ColliderType = {};

	CollisionProfile* m_profile;
	std::list<CBaseCollider*> m_collisionList;

	Vec3 m_Offset;
};

