#pragma once
#include "GameObject.h"

struct ProjectileState
{
	Vec3  Direction{};
	Vec3  Size{};
	float Speed{};
	float Duration{};
	float ElapsedTime{};
	float damage{};
};

class CProjectile : public CGameObject
{
public:
	CProjectile();
	~CProjectile();

public:
	void SetProjectileState(const Vec3 dir, const Vec3 size, const float speed, const float duration, const float elased) { SetProjectileState(ProjectileState(dir, size, speed, duration, elased)); }
	void SetProjectileState(const ProjectileState& state) { m_State = state; }
	void SetCollisionBoxInfo(const Vec3& pos, const Vec3& size, const Vec3& rot);

	ProjectileState& GetStateInfo() { return m_State; }
public:
	virtual void Update(float deltaTime) override;

	virtual void CollisionBegin(CBoxCollider* src, CBoxCollider* dest);

	XMFLOAT3 m_NextAmount{};
public:
	Protocol::ProjectileInfo* ProjectileInfo;
	Protocol::SkillMesh m_meshType;
private:
	ProjectileState m_State{};
};

