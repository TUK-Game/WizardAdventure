#include "pch.h"
#include "PlayerIdleState.h"
#include "GameObject.h"
#include "Animator.h"

void CPlayerIdleState::Enter(CGameObject* entity)
{
#ifdef _DEBUG
	std::cout << "Entering Idle State" << std::endl;
#endif
	std::vector<CGameObject*> objs = entity->GetChild();
	for (const auto o : objs) {
		o->GetAnimator()->Play(L"IDLE");
	}
}

void CPlayerIdleState::Update(CGameObject* entity, float deltaTime)
{

}

void CPlayerIdleState::Exit(CGameObject* entity)
{
#ifdef _DEBUG
	std::cout << "Exiting Idle State" << std::endl;
#endif
}