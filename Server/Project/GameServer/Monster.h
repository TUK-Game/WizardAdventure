#pragma once

#include "Creature.h"

class CMonster : public CCreature
{
public:
	CMonster();
	virtual ~CMonster();

public:
	Protocol::MoveState GetState() { return m_State; }
	Vec3 GetDir() { return m_Dir; }
	bool GetIsActive() { return m_bActive; }
	std::vector<uint32>& GetProjectileIds() { return m_ProjectileIds; }
	Vec3 GetFrontVec();

	void SetIsActive(bool b) { m_bActive = b; }
	void SetState(Protocol::MoveState state) { m_State = state; }
	void SetDir(const Vec3& dir) { m_Dir = dir; }
	void ClearProjectileIds() { m_ProjectileIds.clear(); }
public:
	Protocol::MonsterInfo* MonsterInfo;
public:
	virtual void Update(float deltaTime);

	virtual void CollisionBegin(CBoxCollider* src, CBoxCollider* dest);
	virtual void CollisionEvent(CBoxCollider* src, CBoxCollider* dest);

	virtual void SpawnAttackObject();

protected:
	Protocol::MoveState m_State;
	Vec3 m_Dir;
	bool m_bActive{ false };

	class CMonsterAI* m_Ai = nullptr;

	std::vector<uint32> m_ProjectileIds;
	// temp	
	float m_Speed = 5000.f;

};

