#include "pch.h"
#include "Frustum.h"
#include "Camera.h"

void CFrustum::FinalUpdate()
{
	Matrix projection = CCamera::s_matProjection;
	Matrix view = CCamera::s_matView;
	frustum.CreateFromMatrix(frustum, XMLoadFloat4x4(&projection));
	XMMATRIX xmmtxInversView = XMMatrixInverse(NULL, XMLoadFloat4x4(&view));
	frustum.Transform(frustum, xmmtxInversView);	
	//std::vector<Vec3> worldPos =
	//{
	//	::XMVector3TransformCoord(Vec3(-1.f, 1.f, 0.f), matInv),
	//	::XMVector3TransformCoord(Vec3(1.f, 1.f, 0.f), matInv),
	//	::XMVector3TransformCoord(Vec3(1.f, -1.f, 0.f), matInv),
	//	::XMVector3TransformCoord(Vec3(-1.f, -1.f, 0.f), matInv),
	//	::XMVector3TransformCoord(Vec3(-1.f, 1.f, 1.f), matInv),
	//	::XMVector3TransformCoord(Vec3(1.f, 1.f, 1.f), matInv),
	//	::XMVector3TransformCoord(Vec3(1.f, -1.f, 1.f), matInv),
	//	::XMVector3TransformCoord(Vec3(-1.f, -1.f, 1.f), matInv)
	//};

	//_planes[PLANE_FRONT] = ::XMPlaneFromPoints(worldPos[0], worldPos[1], worldPos[2]); // CW
	//_planes[PLANE_BACK] = ::XMPlaneFromPoints(worldPos[4], worldPos[7], worldPos[5]); // CCW
	//_planes[PLANE_UP] = ::XMPlaneFromPoints(worldPos[4], worldPos[5], worldPos[1]); // CW
	//_planes[PLANE_DOWN] = ::XMPlaneFromPoints(worldPos[7], worldPos[3], worldPos[6]); // CCW
	//_planes[PLANE_LEFT] = ::XMPlaneFromPoints(worldPos[4], worldPos[0], worldPos[7]); // CW
	//_planes[PLANE_RIGHT] = ::XMPlaneFromPoints(worldPos[5], worldPos[6], worldPos[1]); // CCW
}

bool CFrustum::ContainsSphere(const Vec3& pos, float radius) 
{
	for (const Vec4& plane : _planes)
	{
		// n = (a, b, c)
		Vec3 normal = Vec3(plane.x, plane.y, plane.z);

		// ax + by + cz + d > radius
		if (normal.Dot(pos) + plane.w > radius)
			return false;
	}

	return true;
}

bool CFrustum::IsInFrustum(BoundingBox& box)
{
	return frustum.Intersects(box);
}

bool CFrustum::IsInFrustum(BoundingOrientedBox& box)
{
	return frustum.Intersects(box);
}
