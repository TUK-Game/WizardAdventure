#pragma once

#include "GameObject.h"

struct Ablity
{
	int maxHp = 100;
	int currentHp = 100;
	int attack = 10;
	float moveSpeed = 300.f;
	int gold = 1000;

	void TakeDamage(int damage) {
		currentHp = max(currentHp - damage, 0);
	}

	void Heal(int amount) {
		currentHp = min(currentHp + amount, maxHp);
	}

	bool IsDead() const {
		return currentHp <= 0;
	}
};

class CCreature : public CGameObject
{
public:
	CCreature();
	virtual ~CCreature();

public:
	Ablity* GetAbility() const { return m_Stats; }

	void Damaged(int damage) { m_Stats->currentHp -= damage; }

protected:
	Ablity* m_Stats{ nullptr };
};

