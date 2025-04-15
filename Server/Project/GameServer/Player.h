#pragma once

#include "Creature.h"

class CGameSession;
class CRoom;

class CPlayer : public CCreature
{
	friend class CObjectUtil;

public:
	CPlayer();
	virtual ~CPlayer();

public:
	Protocol::MoveState GetState() { return m_State; }
	Protocol::Vector3& GetDir() { return m_Dir; }

	void SetState(Protocol::MoveState state) { m_State = state; }
	void SetDir(const Protocol::Vector3& dir) { m_Dir = dir; }
public:
	Protocol::PlayerInfo* PlayerInfo;

public:
	std::shared_ptr<CGameSession> GetSession() const { return m_Session.lock(); }

	virtual void Update(float deltaTime);

	virtual void CollisionBegin(CBoxCollider* src, CBoxCollider* dest);
	virtual void CollisionEvent(CBoxCollider* src, CBoxCollider* dest);

	const float m_StepSize = 3;
	Protocol::Vector3 m_NextAmount;	
	float m_DashDurtation = 0.2f;
	float m_DashElapsedTime = 0.f;
private:
	std::weak_ptr<CGameSession>	m_Session;
	Protocol::MoveState m_State;
	Protocol::Vector3 m_Dir;
	// temp	
	float m_Speed = 5000.f;
};

