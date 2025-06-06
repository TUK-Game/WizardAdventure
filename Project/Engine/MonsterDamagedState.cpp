#include "pch.h"
#include "MonsterDamagedState.h"
#include "GameObject.h"
#include "Animator.h"
#include "MonsterAI.h"
#include "StateManager.h"
#include "AssetManager.h"

void CMonsterDamagedState::Enter(CGameObject* entity)
{
#ifdef _DEBUG
	std::cout << "Entering Damaged State" << std::endl;
#endif
	std::vector<CGameObject*> objs = entity->GetChild();
	for (const auto o : objs) {
		auto ani = o->GetAnimator();
		if (ani) {
			ani->Play(L"MONSTERHITTED");
			m_DamagedDuration = ani->GetDuration();
		}
	}

}

void CMonsterDamagedState::Update(CGameObject* entity, float deltaTime)
{
	//m_ElapsedTime += deltaTime;
	//if (m_ElapsedTime >= m_DamagedDuration)
	//{
	//	entity->GetStateManager()->HandleEvent(entity, "EndDamaged");
	//}
}

void CMonsterDamagedState::Exit(CGameObject* entity)
{
#ifdef _DEBUG
	std::cout << "Exiting Damaged State" << std::endl;
#endif
}