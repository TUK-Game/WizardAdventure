#pragma once
class CLevelCollision
{
public:
	CLevelCollision();
	~CLevelCollision();

public:
	std::vector<class CBoxCollider*>& GetLayerCollider(uint32 layer) { return m_vecCollider[layer]; }

public:
	void AddCollider(class CBoxCollider* collider, ECollision_Channel channel);
	void Collision();
	bool CollisionWithWall(CBoxCollider* collider);
	bool CollisionWithPlayer(CBoxCollider* collider);
	bool CollisionWithNPC(CBoxCollider* collider);
	bool CollisionWithOnlyChannel(ECollision_Channel channel, CBoxCollider* collider);
	bool MeshBoxCheck(const std::vector<WorldTriangle>& triangles, const BoundingOrientedBox& box);
	bool IntersectTriangleWithBoundingBox(const WorldTriangle& tri, const BoundingOrientedBox& box);
	void ProjectTriangle(const Vector3& axis, const Vector3& v0, const Vector3& v1, const Vector3& v2, float& outMin, float& outMax);

private:
	std::array<std::vector<class CBoxCollider*>, (int)ECollision_Channel::Max>		m_vecCollider;
};

