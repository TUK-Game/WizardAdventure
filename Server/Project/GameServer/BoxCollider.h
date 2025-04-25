#pragma once

class CBoxCollider
{
public:
	CBoxCollider();
	virtual ~CBoxCollider();

public:
	BoundingOrientedBox& GetBoundingBox() { return m_BoundingBox; }
	CGameObject* GetOwner() { return m_Owner; }
	const CollisionProfile* GetProfile() const { return m_profile; }
	const std::vector<WorldTriangle>& GetWorldTriangle() { return m_Triangles; }
	const Vec3& GetGateDir() const { return m_GateDir; }

	void SetWorldTriangle(const std::vector<WorldTriangle>& triangles) { m_Triangles = triangles; }
	void SetOwner(class CGameObject* owner) { m_Owner = owner; }
	void SetGateDir(const Vec3& dir) { m_GateDir = dir; }
public:
	bool Collision(CBoxCollider* dest);
	virtual void SetBoxInfo(const Vec3& centerPos, const Vec3& size, const Vec3& rotation, const Vec3& offset = Vec3(0.f, 0.f, 0.f));
	virtual void SetBoxPosAndSize(const Vec3& centerPos, const Vec3& size);
	void SetBoxHeight(float height);

	void CreateCollisionProfile(std::string name, ECollision_Channel channel);
	void SetCollisionProfile(const std::string& name);
	void AddCollisionList(CBoxCollider* collider);
	bool CheckCollisionList(CBoxCollider* collider);
	void DeleteCollisionList(CBoxCollider* collider);
	void ClearCollisionList();
	void CallCollisionBegin(CBoxCollider* dest);
	void CollisionEvent(CBoxCollider* dest);
	void CallCollisionEnd(CBoxCollider* dest);
public:
	virtual void Update();


protected:
	BoundingOrientedBox m_BoundingBox;
	CollisionProfile* m_profile = nullptr;
	std::list<CBoxCollider*> m_collisionList;
	XMFLOAT3 m_Offset;
	ECollision_Channel m_Channel;
	Vec3 m_GateDir;
	
	class CGameObject* m_Owner = nullptr;
	std::vector<WorldTriangle> m_Triangles;
};

