#include "pch.h"
#include "BaseCollider.h"
#include "Transform.h"

CBaseCollider::CBaseCollider(EColliderType colliderType)
	: CComponent(EComponent_Type::Collider), m_ColliderType(colliderType)
{
}

CBaseCollider::~CBaseCollider()
{
}
