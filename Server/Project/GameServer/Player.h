#pragma once

#include "Creature.h"

class CGameSession;
class CRoom;

enum class EAttribution
{
	FIRE,
	ICE,
	LIGHTNING,

	MAX
};

class CPlayer : public CCreature
{
	friend class CObjectUtil;

public:
	CPlayer();
	virtual ~CPlayer();

public:
	Protocol::MoveState GetState() { return m_State; }
	Protocol::Vector3& GetDir() { return m_Dir; }
	EAttribution GetAttribution() { return m_attribution; }
	Vec3 GetSafePosition() { return m_SafePos; }

	void SetState(Protocol::MoveState state) { m_State = state; }
	void SetAblity(int maxHp, int currentHp, int attack, float speed) { Ablity{ maxHp, currentHp, attack, speed }; }
	void SetDir(const Protocol::Vector3& dir) { m_Dir = dir; }
	void SetAttribute(const EAttribution attri) { m_attribution = attri; }
	void SetSafePosition(const Vec3& pos) { m_SafePos = pos; }
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
	Vector3 m_SafePos;

	EAttribution m_attribution = EAttribution::FIRE;

	// temp	
	float m_Speed = 5000.f;
};

