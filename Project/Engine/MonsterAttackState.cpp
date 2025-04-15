#include "pch.h"
#include "MonsterAttackState.h"
#include "Animator.h"
#include "MonsterAI.h"
#include "Transform.h"
#include "StateManager.h"


void CMonsterAttackState::Enter(CGameObject* entity)
{
#ifdef _DEBUG;
	std::cout << "Entering Monster Attack State" << std::endl;
#endif
    std::vector<CGameObject*> objs = entity->GetChild();
    for (const auto o : objs) {
        CAnimator* ani = o->GetAnimator();
        ani->Play(L"MONSTERATTACK1");
        m_AttackDuration = ani->GetDuration();

    }
    m_ElapsedTime = 0.f;
}

void CMonsterAttackState::Update(CGameObject* entity, float deltaTime)
{
    //m_ElapsedTime += deltaTime;
    //if (m_ElapsedTime >= m_AttackDuration)
    //    entity->GetStateManager()->HandleEvent(entity, "Idle");
}

void CMonsterAttackState::Exit(CGameObject* entity)
{
#ifdef _DEBUG;
    std::cout << "Exiting Monster Attack State" << std::endl;
#endif
}