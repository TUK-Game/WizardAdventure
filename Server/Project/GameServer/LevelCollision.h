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
	bool MeshBoxCheck(const std::vector<WorldTriangle>& triangles, const BoundingBox& box);
	bool IntersectTriangleWithBoundingBox(const WorldTriangle& tri, const BoundingBox& box);
	void ProjectTriangle(const Vector3& axis, const Vector3& v0, const Vector3& v1, const Vector3& v2, float& outMin, float& outMax);

private:
	std::array<std::vector<class CBoxCollider*>, (int)ECollision_Channel::Max>		m_vecCollider;
};

