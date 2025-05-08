#include "pch.h"
#include "PlayerAttackEState.h"
#include "Player.h"
#include "Animator.h"
#include "InputManager.h"
#include "StateManager.h"
#include "SkillData.h"

void CPlayerAttackEState::Enter(CGameObject* entity)
{
#ifdef _DEBUG;
	std::cout << "Entering AttackE State" << std::endl;
#endif
    CPlayer* player = dynamic_cast<CPlayer*>(entity);
    if (ESkillType::None == player->GetSkillManager()->GetEquippedSkill(ESkillSlot::E))
    {
        entity->GetStateManager()->HandleEvent(entity, "EndAttack");
        return;
    }

    const SkillInfo& skill = player->GetSkillManager()->GetSkill(ESkillSlot::E)->GetSkillInfo();
    std::vector<CGameObject*> objs = entity->GetChild();
    for (const auto o : objs) {
        CAnimator* ani = o->GetAnimator();
        ani->Play(skill.animationName);
        m_AttackDuration = ani->GetDuration();
    }
    player->Attack(ESkillSlot::E, m_AttackDuration * skill.explosionTime);
    m_ElapsedTime = 0.f;
}

void CPlayerAttackEState::Update(CGameObject* entity, float deltaTime)
{
    m_ElapsedTime += deltaTime;
    if (m_ElapsedTime >= m_AttackDuration)
        entity->GetStateManager()->HandleEvent(entity, "EndAttack");
}

void CPlayerAttackEState::Exit(CGameObject* entity)
{
#ifdef _DEBUG;
    std::cout << "Exiting AttackE State" << std::endl;
#endif
}