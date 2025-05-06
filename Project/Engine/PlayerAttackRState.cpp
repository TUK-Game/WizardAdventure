#include "pch.h"
#include "PlayerAttackRState.h"
#include "Player.h"
#include "Animator.h"
#include "InputManager.h"
#include "StateManager.h"

void CPlayerAttackRState::Enter(CGameObject* entity)
{
#ifdef _DEBUG;
	std::cout << "Entering AttackR State" << std::endl;
#endif
    std::vector<CGameObject*> objs = entity->GetChild();
    for (const auto o : objs) {
        CAnimator* ani = o->GetAnimator();
        ani->Play(L"MAGEATTACK3");
        m_AttackDuration = ani->GetDuration();
    }
    m_ElapsedTime = 0.f;
    CPlayer* player = dynamic_cast<CPlayer*>(entity);

    if (ESkillType::None == player->GetSkillManager()->GetEquippedSkill(ESkillSlot::R))
        entity->GetStateManager()->HandleEvent(entity, "EndAttack");
    player->Attack(ESkillSlot::R, m_AttackDuration * .2f);
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