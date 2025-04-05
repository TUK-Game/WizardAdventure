#pragma once

class CBoxCollider
{
public:
	CBoxCollider();
	virtual ~CBoxCollider();

public:
	BoundingBox& GetBoundingBox() { return m_BoundingBox; }
	CGameObject* GetOwner() { return m_Owner; }
	const CollisionProfile* GetProfile() const { return m_profile; }

	void SetOwner(class CGameObject* owner) { m_Owner = owner; }
public:
	bool Collision(CBoxCollider* dest);
	void SetBoxInfo(XMFLOAT3 centerPos, XMFLOAT3 size, XMFLOAT3 offset = XMFLOAT3(0.f, 0.f, 0.f));

	void CreateCollisionProfile(std::string name, ECollision_Channel channel);
	void SetCollisionProfile(const std::string& name);
	void AddCollisionList(CBoxCollider* collider);
	bool CheckCollisionList(CBoxCollider* collider);
	void DeleteCollisionList(CBoxCollider* collider);
	void ClearCollisionList();
	void CallCollisionBegin(CBoxCollider* dest);
	void CallCollisionEnd(CBoxCollider* dest);
public:
	void Update();


private:
	BoundingBox m_BoundingBox;
	CollisionProfile* m_profile = nullptr;
	std::list<CBoxCollider*> m_collisionList;
	XMFLOAT3 m_Offset;
	
	class CGameObject* m_Owner = nullptr;
};

