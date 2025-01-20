#pragma once
class CLevelCollision
{
public:
	CLevelCollision();
	~CLevelCollision();

public:
	void AddCollider(class CBaseCollider* collider);
	void Collision();

private:
	std::vector<class CBaseCollider*>		m_vecCollider;
};

