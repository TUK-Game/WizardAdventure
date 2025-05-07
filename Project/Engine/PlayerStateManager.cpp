#include "pch.h"
#include "PlayerStateManager.h"
#include "GameObject.h"
#include "NetworkManager.h"
#include "ServerSession.h"
#include "Player.h"
#include "SkillManager.h"

CPlayerStateManager::~CPlayerStateManager()
{
}

void CPlayerStateManager::ChangeState(CGameObject* entity, EState_Type newState)
{
	if (m_CurrentState)
	{
		m_CurrentState->Exit(entity);
	}

	m_CurrentState = m_States[newState];

	if (m_CurrentState)
		m_CurrentState->Enter(entity);
}

void CPlayerStateManager::HandleEvent(CGameObject* entity, const std::string& event)
{
	if (m_CurrentState)
	{
		EState_Type currentType = m_CurrentState->GetStateType();
		if (m_TransitionTable[currentType].find(event) != m_TransitionTable[currentType].end())
		{
			EState_Type nextType = m_TransitionTable[currentType][event];
			if (IsCoolTime(entity, nextType))
			{
				ChangeState(entity, nextType);
				CNetworkManager::GetInst()->s_GameSession->OnActPlayer();
			}
		}
	}
}

bool CPlayerStateManager::IsCoolTime(CGameObject* entity, EState_Type next)
{
	const auto& skillManager = dynamic_cast<CPlayer*>(entity)->GetSkillManager();
	switch (next)
	{
	case EState_Type::Attack_Q:
	{
		if (!CheckSkill(skillManager, ESkillSlot::Q))
			return false;
	}
	break;
	case EState_Type::Attack_E:
	{
		if (!CheckSkill(skillManager, ESkillSlot::E))
			return false;
	}
	break;
	case EState_Type::Attack_R:
	{
		if (!CheckSkill(skillManager, ESkillSlot::R))
			return false;
	}
	break;
	case EState_Type::Attack_LButton:
	{
		if (!CheckSkill(skillManager, ESkillSlot::LButton))
			return false;
	}
	break;
	case EState_Type::Attack_RButton:
	{
		if (!CheckSkill(skillManager, ESkillSlot::RButton))
			return false;
	}
	break;
	}

	return true;
}

bool CPlayerStateManager::CheckSkill(CSkillManager* manager, ESkillSlot slot)
{
	if (ESkillType::None == manager->GetEquippedSkill(slot) || manager->GetSkillCooldown(slot) > 0.f)
		return false;

	return true;
}
