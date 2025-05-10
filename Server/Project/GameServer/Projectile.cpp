#include "pch.h"
#include "Projectile.h"
#include "BoxCollider.h"
#include "ProjectileCollider.h"

CProjectile::CProjectile()
{
	ProjectileInfo = new Protocol::ProjectileInfo();
	ObjectInfo = new Protocol::ObjectInfo();
	PosInfo = new Protocol::PosInfo();

	ObjectInfo->set_allocated_pos_info(PosInfo);
	ProjectileInfo->set_allocated_object_info(ObjectInfo);

	m_BoxCollider = new CProjectileCollider;
	m_BoxCollider->SetOwner(this);
	m_BoxCollider->SetCollisionProfile("Projectile");
}

CProjectile::~CProjectile()
{
	if (ObjectInfo)
		delete ObjectInfo;
}

void CProjectile::SetCollisionBoxInfo(const Vec3& pos, const Vec3& size, const Vec3& rot)
{
	m_BoxCollider->SetBoxInfo(pos, size, rot);
}

void CProjectile::Update(float deltaTime)
{
	CGameObject::Update(deltaTime);
}

void CProjectile::CollisionBegin(CBoxCollider* src, CBoxCollider* dest)
{
	if(m_bCollisionExplosion)
	{
		ProjectileInfo->set_state(Protocol::COLLISION);
	}
}
