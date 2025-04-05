#pragma once
class CLevelCollision
{
public:
	CLevelCollision();
	~CLevelCollision();

public:
	void AddCollider(class CBoxCollider* collider);
	void Collision();

private:
	std::vector<class CBoxCollider*>		m_vecCollider;
};

