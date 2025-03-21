#include "pch.h"
#include "PlayerIdleState.h"
#include "GameObject.h"
#include "Animator.h"

void CPlayerIdleState::Enter(CGameObject* entity)
{
	std::cout << "Entering Idle State" << std::endl;
	std::vector<CGameObject*> objs = entity->GetChild();
	for (const auto o : objs) {
		if(o->GetAnimator())
			o->GetAnimator()->Play(L"IDLE");
	}
}

void CPlayerIdleState::Update(CGameObject* entity, float deltaTime)
{

}

void CPlayerIdleState::Exit(CGameObject* entity)
{
	std::cout << "Exiting Idle State" << std::endl;
}