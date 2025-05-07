#include "pch.h"
#include "PlayerAttackQState.h"
#include "Player.h"
#include "Animator.h"
#include "InputManager.h"
#include "StateManager.h"
#include "SkillData.h"

void CPlayerAttackQState::Enter(CGameObject* entity)
{
#ifdef _DEBUG;
	std::cout << "Entering AttackQ State" << std::endl;
#endif
    CPlayer* player = dynamic_cast<CPlayer*>(entity);
    if (ESkillType::None == player->GetSkillManager()->GetEquippedSkill(ESkillSlot::Q))
    {
        entity->GetStateManager()->HandleEvent(entity, "EndAttack");
        return;
    }

    const SkillInfo& skill = player->GetSkillManager()->GetSkill(ESkillSlot::Q)->GetSkillInfo();
    std::vector<CGameObject*> objs = entity->GetChild();
    for (const auto o : objs) {
        CAnimator* ani = o->GetAnimator();
        ani->Play(skill.animationName);
        m_AttackDuration = ani->GetDuration();
    }
    player->Attack(ESkillSlot::Q, m_AttackDuration * skill.explosionTime);
    m_ElapsedTime = 0.f;
}

void CPlayerAttackQState::Update(CGameObject* entity, float deltaTime)
{
    m_ElapsedTime += deltaTime;
    if (m_ElapsedTime >= m_AttackDuration)
        entity->GetStateManager()->HandleEvent(entity, "EndAttack");
}

void CPlayerAttackQState::Exit(CGameObject* entity)
{
#ifdef _DEBUG;
    std::cout << "Exiting AttackQ State" << std::endl;
#endif
}