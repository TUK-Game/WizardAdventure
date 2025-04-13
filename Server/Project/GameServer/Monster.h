#pragma once

#include "Creature.h"

class CMonster : public CCreature
{
public:
	CMonster();
	virtual ~CMonster();

public:
	Protocol::MoveState GetState() { return m_State; }
	Protocol::Vector3& GetDir() { return m_Dir; }

	void SetState(Protocol::MoveState state) { m_State = state; }
	void SetDir(const Protocol::Vector3& dir) { m_Dir = dir; }

public:
	Protocol::MonsterInfo* MonsterInfo;
public:
	virtual void Update();	

	virtual void CollisionBegin(CBoxCollider* src, CBoxCollider* dest);
	virtual void CollisionEvent(CBoxCollider* src, CBoxCollider* dest);


private:
	Protocol::MoveState m_State;
	Protocol::Vector3 m_Dir;
	// temp	
	float m_Speed = 5000.f;
};

