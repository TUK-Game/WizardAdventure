#pragma once
class CLevelCollision
{
public:
	CLevelCollision();
	~CLevelCollision();

public:
	void AddCollider(class CBoxCollider* collider, ECollision_Channel channel);
	void Collision();
	bool CollisionWithWall(CBoxCollider* collider);

private:
	std::array<std::vector<class CBoxCollider*>, (int)ECollision_Channel::Max>		m_vecCollider;
};

