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
	std::shared_ptr<CGameSession> GetSession() const { return m_Session.lock(); }

private:
	std::weak_ptr<CGameSession>	m_Session;
};

