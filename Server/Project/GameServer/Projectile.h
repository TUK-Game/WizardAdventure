#pragma once
#include "GameObject.h"

struct ProjectileState
{
	Vec3  Direction{};
	Vec3  Size{};
	float Speed{};
	float Duration{};
	float ElapsedTime{};
	int damage{};
};
class CProjectileCollider;

class CProjectile : public CGameObject
{
public:
	CProjectile();
	~CProjectile();

public:
	void SetProjectileState(const Vec3 dir, const Vec3 size, const float speed, const float duration, const float elased) { SetProjectileState(ProjectileState(dir, size, speed, duration, elased)); }
	void SetProjectileState(const ProjectileState& state) { m_Stats = state; }
	void SetCollisionBoxInfo(const Vec3& pos, const Vec3& size, const Vec3& rot);
	void SetCollisionExplosion(bool b) { m_bCollisionExplosion = b; }
	void SetMeshSize(const Vec3& size) { m_MeshSize = size; }

	Vec3 GetMeshSize() { return m_MeshSize; }
	bool GetCollisionExplosion() { return m_bCollisionExplosion; }
	ProjectileState& GetStateInfo() { return m_Stats; }
	int GetAttack() { return m_Stats.damage; }
public:
	virtual void Update(float deltaTime) override;

	virtual void CollisionBegin(CBoxCollider* src, CBoxCollider* dest);

	XMFLOAT3 m_NextAmount{};
public:
	Protocol::ProjectileInfo* ProjectileInfo;
	Protocol::SkillMesh m_meshType;
protected:
	bool m_bCollisionExplosion{};
	ProjectileState m_Stats{};
	CProjectileCollider* m_box;
};

