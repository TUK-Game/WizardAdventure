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
}

bool CFrustum::IsInFrustum(BoundingBox& box)
{
	return frustum.Intersects(box);
}

bool CFrustum::IsInFrustum(BoundingOrientedBox& box)
{
	return frustum.Intersects(box);
}
