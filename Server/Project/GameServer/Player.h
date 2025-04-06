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
	Protocol::PlayerInfo* PlayerInfo;

public:
	std::shared_ptr<CGameSession> GetSession() const { return m_Session.lock(); }

	virtual void Update();

	virtual void CollisionBegin(CBoxCollider* src, CBoxCollider* dest);
	virtual void CollisionEvent(CBoxCollider* src, CBoxCollider* dest);

	bool block[4]{};

private:
	std::weak_ptr<CGameSession>	m_Session;
};

