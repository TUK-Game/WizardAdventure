#include "pch.h"
#include "PlayerAttackRState.h"
#include "Player.h"
#include "Animator.h"
#include "InputManager.h"
#include "StateManager.h"
#include "SkillData.h"

void CPlayerAttackRState::Enter(CGameObject* entity)
{
#ifdef _DEBUG;
	std::cout << "Entering AttackR State" << std::endl;
#endif
    CPlayer* player = dynamic_cast<CPlayer*>(entity);
    if (ESkillType::None == player->GetSkillManager()->GetEquippedSkill(ESkillSlot::R))
    {
        entity->GetStateManager()->HandleEvent(entity, "EndAttack");
        return;
    }

    const SkillInfo& skill = player->GetSkillManager()->GetSkill(ESkillSlot::R)->GetSkillInfo();
    std::vector<CGameObject*> objs = entity->GetChild();
    for (const auto o : objs) {
        CAnimator* ani = o->GetAnimator();
        ani->Play(skill.animationName);
        m_AttackDuration = ani->GetDuration();
    }
    player->Attack(ESkillSlot::R, m_AttackDuration * skill.explosionTime);
    m_ElapsedTime = 0.f;
}

void CPlayerAttackRState::Update(CGameObject* entity, float deltaTime)
{

    m_ElapsedTime += deltaTime;
    if (m_ElapsedTime >= m_AttackDuration)
        entity->GetStateManager()->HandleEvent(entity, "EndAttack");
}

void CPlayerAttackRState::Exit(CGameObject* entity)
{
#ifdef _DEBUG;
    std::cout << "Exiting AttackR State" << std::endl;
#endif
}