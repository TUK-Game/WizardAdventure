#pragma once
enum PLANE_TYPE : UINT8
{
	PLANE_FRONT,
	PLANE_BACK,
	PLANE_UP,
	PLANE_DOWN,
	PLANE_LEFT,
	PLANE_RIGHT,

	PLANE_END
};

class CFrustum
{
public:
	void FinalUpdate();
	bool ContainsSphere(const Vec3& pos, float radius);
	bool IsInFrustum(BoundingBox& box);
	bool IsInFrustum(BoundingOrientedBox& box);
private:
	BoundingFrustum frustum;
};

