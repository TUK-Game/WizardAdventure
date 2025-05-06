#include "pch.h"
#include "PlayerAttackRButtonState.h"
#include "Player.h"
#include "Animator.h"
#include "InputManager.h"
#include "StateManager.h"

void CPlayerAttackRButtonState::Enter(CGameObject* entity)
{
#ifdef _DEBUG;
	std::cout << "Entering AttackL State" << std::endl;
#endif
    std::vector<CGameObject*> objs = entity->GetChild();
    for (const auto o : objs) {
        CAnimator* ani = o->GetAnimator();
        ani->Play(L"MAGEATTACK2");
        m_AttackDuration = ani->GetDuration();
    }
    m_ElapsedTime = 0.f;
    CPlayer* player = dynamic_cast<CPlayer*>(entity);

    if (ESkillType::None == player->GetSkillManager()->GetEquippedSkill(ESkillSlot::RButton))
        entity->GetStateManager()->HandleEvent(entity, "EndAttack");
    player->Attack(ESkillSlot::RButton);
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