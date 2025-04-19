#pragma once

#include "GameObject.h"

class CCreature : public CGameObject
{
public:
	CCreature();
	virtual ~CCreature();

protected:
	bool m_bDeath{};
};

