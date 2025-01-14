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
	virtual bool Intersects() { return false; }
	virtual bool IsFrustum(class CFrustum frustum);

public:
	virtual void FinalUpdate() override;
		
public:
	virtual CBoxCollider* Clone() override { return new CBoxCollider(*this); }

private:
	BoundingBox m_BoundingBox;
};

