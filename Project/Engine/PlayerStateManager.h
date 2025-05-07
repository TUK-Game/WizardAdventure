#pragma once
#include "StateManager.h"

class CPlayerStateManager :
	public CStateManager
{
public:
	CPlayerStateManager() {}
	virtual ~CPlayerStateManager();
public:
	virtual void ChangeState(CGameObject* entity, EState_Type newState);
	virtual void HandleEvent(CGameObject* entity, const std::string& event);
	bool IsCoolTime(CGameObject* entity, EState_Type next);
	bool CheckSkill(class CSkillManager* manager, ESkillSlot slot);
};

