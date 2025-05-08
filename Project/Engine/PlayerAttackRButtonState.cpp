#include "pch.h"
#include "PlayerAttackRButtonState.h"
#include "Player.h"
#include "Animator.h"
#include "InputManager.h"
#include "StateManager.h"
#include "SkillData.h"

void CPlayerAttackRButtonState::Enter(CGameObject* entity)
{
#ifdef _DEBUG;
	std::cout << "Entering AttackL State" << std::endl;
#endif
    CPlayer* player = dynamic_cast<CPlayer*>(entity);
    if (ESkillType::None == player->GetSkillManager()->GetEquippedSkill(ESkillSlot::RButton))
    {
        entity->GetStateManager()->HandleEvent(entity, "EndAttack");
        return;
    }

    const SkillInfo& skill = player->GetSkillManager()->GetSkill(ESkillSlot::RButton)->GetSkillInfo();
    std::vector<CGameObject*> objs = entity->GetChild();
    for (const auto o : objs) {
        CAnimator* ani = o->GetAnimator();
        ani->Play(skill.animationName);
        m_AttackDuration = ani->GetDuration();
    }
    player->Attack(ESkillSlot::RButton, m_AttackDuration * skill.explosionTime);
    m_ElapsedTime = 0.f;
}

void CPlayerAttackRButtonState::Update(CGameObject* entity, float deltaTime)
{
    m_ElapsedTime += deltaTime;
    if (m_ElapsedTime >= m_AttackDuration)
        entity->GetStateManager()->HandleEvent(entity, "EndAttack");
}

void CPlayerAttackRButtonState::Exit(CGameObject* entity)
{
#ifdef _DEBUG;
    std::cout << "Exiting AttackL State" << std::endl;
#endif
}