#include "pch.h"
#include "MonsterDeathState.h"
#include "GameObject.h"
#include "Animator.h"
#include "MonsterAI.h"

void CMonsterDeathState::Enter(CGameObject* entity)
{
#ifdef _DEBUG
	std::cout << "Entering Death State" << std::endl;
#endif
	std::vector<CGameObject*> objs = entity->GetChild();
	for (const auto o : objs) {

		auto ani = o->GetAnimator();
		if (ani) {
			ani->Play(L"MONSTERDIE");
			m_DeathDuration = ani->GetDuration();
		}
		
	}

}

void CMonsterDeathState::Update(CGameObject* entity, float deltaTime)
{
	//m_ElapsedTime += deltaTime;
	//if (m_ElapsedTime >= m_DeathDuration)
	//{
	//	// Die
	//}
}

void CMonsterDeathState::Exit(CGameObject* entity)
{
#ifdef _DEBUG
	std::cout << "Exiting Death State" << std::endl;
#endif
}