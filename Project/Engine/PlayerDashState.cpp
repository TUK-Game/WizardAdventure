#include "pch.h"
#include "PlayerDashState.h"
#include "Player.h"
#include "Animator.h"
#include "InputManager.h"
#include "StateManager.h"

void CPlayerDashState::Enter(CGameObject* entity)
{
#ifdef _DEBUG;
	std::cout << "Entering Dash State" << std::endl;
#endif
    std::vector<CGameObject*> objs = entity->GetChild();
    for (const auto o : objs) {
        o->GetAnimator()->Play(L"DASH");
    }
    m_ElapsedTime = 0.f;
}

void CPlayerDashState::Update(CGameObject* entity, float deltaTime)
{
}

void CPlayerDashState::Exit(CGameObject* entity)
{
#ifdef _DEBUG;
    std::cout << "Exiting Dash State" << std::endl;
#endif
}