#include "pch.h"
#include "PlayerAttackEState.h"
#include "Player.h"
#include "Animator.h"
#include "InputManager.h"
#include "StateManager.h"

void CPlayerAttackEState::Enter(CGameObject* entity)
{
#ifdef _DEBUG;
	std::cout << "Entering AttackE State" << std::endl;
#endif
    std::vector<CGameObject*> objs = entity->GetChild();
    for (const auto o : objs) {
        CAnimator* ani = o->GetAnimator();
        ani->Play(L"MAGEATTACK4");
        m_AttackDuration = ani->GetDuration();
    }
    m_ElapsedTime = 0.f;
    CPlayer* player = dynamic_cast<CPlayer*>(entity);
    if (ESkillType::None == player->GetSkillManager()->GetEquippedSkill(ESkillSlot::E))
        entity->GetStateManager()->HandleEvent(entity, "EndAttack");

    player->Attack(ESkillSlot::E, m_AttackDuration * 0.615f);
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