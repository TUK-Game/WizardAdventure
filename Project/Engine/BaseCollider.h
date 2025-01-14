#pragma once
#include "Component.h"

enum class EColliderType
{
	Box,
	OrientedBox,
	Sphere,
};

class CBaseCollider : public CComponent
{
public:
	CBaseCollider(EColliderType colliderType);
	virtual ~CBaseCollider();

public:
	EColliderType GetColldiderType() { return m_ColliderType; }

public:
	virtual bool Intersects() = 0;
	virtual bool IsFrustum(class CFrustum frustum) = 0;

public:
	EColliderType m_ColliderType = {};
};

