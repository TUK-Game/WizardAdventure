#include "pch.h"
#include "PlayerAttackLButtonState.h"
#include "Player.h"
#include "Animator.h"
#include "InputManager.h"
#include "StateManager.h"

void CPlayerAttackLButtonState::Enter(CGameObject* entity)
{
#ifdef _DEBUG;
	std::cout << "Entering AttackL State" << std::endl;
#endif
    std::vector<CGameObject*> objs = entity->GetChild();
    for (const auto o : objs) {
        CAnimator* ani = o->GetAnimator();
        ani->Play(L"MAGEATTACK1");
        m_AttackDuration = ani->GetDuration();
    }
    m_ElapsedTime = 0.f;
    CPlayer* player = dynamic_cast<CPlayer*>(entity);
    player->Attack(3);
}

void CPlayerAttackLButtonState::Update(CGameObject* entity, float deltaTime)
{
    m_ElapsedTime += deltaTime;
    if (m_ElapsedTime >= m_AttackDuration)
        entity->GetStateManager()->HandleEvent(entity, "EndAttack");
}

void CPlayerAttackLButtonState::Exit(CGameObject* entity)
{
#ifdef _DEBUG;
    std::cout << "Exiting AttackL State" << std::endl;
#endif
}