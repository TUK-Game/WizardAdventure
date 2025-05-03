#include "pch.h"
#include "MonsterSpawnState.h"
#include "GameObject.h"
#include "Animator.h"
#include "MonsterAI.h"

void CMonsterSpawnState::Enter(CGameObject* entity)
{
#ifdef _DEBUG
	std::cout << "Entering Spawn State" << std::endl;
#endif
	std::vector<CGameObject*> objs = entity->GetChild();
	for (const auto o : objs) {
		o->GetAnimator()->Play(L"MOSNTERSPAWN");
	}
}

void CMonsterSpawnState::Update(CGameObject* entity, float deltaTime)
{
}

void CMonsterSpawnState::Exit(CGameObject* entity)
{
#ifdef _DEBUG
	std::cout << "Exiting Spawn State" << std::endl;
#endif
}