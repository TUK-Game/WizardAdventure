#pragma once
#include "BaseCollider.h"

class CBoxCollider : public CBaseCollider
{
public:
	CBoxCollider();
	virtual ~CBoxCollider();

public:
	BoundingBox& GetBoundingBox() { return m_BoundingBox; }

public:
	virtual bool Intersects(Vec4 rayOrigin, Vec4 rayDir, OUT float& distance);
	virtual bool IsFrustum(class CFrustum frustum);
	virtual bool Collision(CBaseCollider* dest);
	virtual void SetMaxMinPos(Vec4 centerPos, Vec3 maxPos, Vec3 minPos);

public:
	virtual void FinalUpdate() override;
		
public:
	virtual CBoxCollider* Clone() override { return new CBoxCollider(*this); }

private:
	BoundingBox m_BoundingBox;
};

