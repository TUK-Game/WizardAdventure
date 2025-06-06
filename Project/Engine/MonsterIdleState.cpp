#include "pch.h"
#include "MonsterIdleState.h"
#include "GameObject.h"
#include "Animator.h"
#include "MonsterAI.h"

void CMonsterIdleState::Enter(CGameObject* entity)
{
#ifdef _DEBUG
	std::cout << "Entering Idle State" << std::endl;
#endif
	std::vector<CGameObject*> objs = entity->GetChild();
	for (const auto o : objs) {
		auto ani = o->GetAnimator();
		if (ani)
			ani->Play(L"MONSTERIDLE");
	}

}

void CMonsterIdleState::Update(CGameObject* entity, float deltaTime)
{
	//entity->GetMonsterAI()->RotateToTarget(deltaTime);
}

void CMonsterIdleState::Exit(CGameObject* entity)
{
#ifdef _DEBUG
	std::cout << "Exiting Idle State" << std::endl;
#endif
}