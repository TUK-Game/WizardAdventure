#include "pch.h"
#include "PlayerDeathState.h"
#include "Player.h"
#include "Animator.h"
#include "StateManager.h"
#include "NetworkManager.h"

void CPlayerDeathState::Enter(CGameObject* entity)
{
#ifdef _DEBUG
    std::cout << "Entering Death State" << std::endl;
#endif

    m_ElapsedTime = 0.f;

    std::vector<CGameObject*> objs = entity->GetChild();
    for (auto o : objs) {
        o->GetAnimator()->Play(L"FallingDeath"); 
        //o->GetAnimator()->Play(L"FlyingDeath"); 
        m_DeathDuration = o->GetAnimator()->GetDuration();
    }

#ifndef DEBUG_SOLOPLAY
#endif
}

void CPlayerDeathState::Update(CGameObject* entity, float deltaTime)
{
    m_ElapsedTime += deltaTime;

    if (m_ElapsedTime >= m_DeathDuration)
    {
        entity->GetStateManager()->HandleEvent(entity, "EndDeath");
    }
}

void CPlayerDeathState::Exit(CGameObject* entity)
{
#ifdef _DEBUG
    std::cout << "Exiting Death State" << std::endl;
#endif
}