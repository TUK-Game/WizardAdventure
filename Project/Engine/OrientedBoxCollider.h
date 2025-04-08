#pragma once
#include "BaseCollider.h"

class COrientedBoxCollider : public CBaseCollider
{
public:
	COrientedBoxCollider();
	virtual ~COrientedBoxCollider();

public:
	BoundingOrientedBox& GetBoundingOrietedBox() { return m_BoundingOrientedBox; }

public:
	virtual bool Intersects(Vec4 rayOrigin, Vec4 rayDir, OUT float& distance);
	virtual bool IsFrustum(class CFrustum& frustum);

public:
	virtual void FinalUpdate() override;

public:
	virtual COrientedBoxCollider* Clone() override { return new COrientedBoxCollider(*this); }

private:
	BoundingOrientedBox m_BoundingOrientedBox;
};

