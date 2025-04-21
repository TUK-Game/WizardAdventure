#include "pch.h"
#include "PlayerAttackQState.h"
#include "Player.h"
#include "Animator.h"
#include "InputManager.h"
#include "StateManager.h"

void CPlayerAttackQState::Enter(CGameObject* entity)
{
#ifdef _DEBUG;
	std::cout << "Entering AttackQ State" << std::endl;
#endif
    std::vector<CGameObject*> objs = entity->GetChild();
    for (const auto o : objs) {
        CAnimator* ani = o->GetAnimator();
        ani->Play(L"MAGEATTACK5");
        m_AttackDuration = ani->GetDuration();
    }
    m_ElapsedTime = 0.f;
    CPlayer* player = dynamic_cast<CPlayer*>(entity);
    player->Attack(0);
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