#include "pch.h"
#include "MonsterChaseState.h"
#include "Animator.h"
#include "MonsterAI.h"
#include "Transform.h"
#include "StateManager.h"


void CMonsterChaseState::Enter(CGameObject* entity)
{
#ifdef _DEBUG;
	std::cout << "Entering Monster Chase State" << std::endl;
#endif
    std::vector<CGameObject*> objs = entity->GetChild();
    for (const auto o : objs) {
        o->GetAnimator()->Play(L"MONSTERWALK");
    }
}

void CMonsterChaseState::Update(CGameObject* entity, float deltaTime)
{

    CGameObject* target = entity->GetMonsterAI()->GetTarget();  
    if (!target) return;

    CTransform* myTransform = entity->GetTransform();
    CTransform* targetTransform = target->GetTransform();

    Vec3 myPos = myTransform->GetRelativePosition();
    Vec3 targetPos = targetTransform->GetWorldPosition();

    Vec3 dir = targetPos - myPos;
    float dist = dir.Length();

    if (dist < 1.f) return; // 너무 가까우면 이동 생략
    dir.Normalize();

    Vec3 nextPos = myPos + dir * 300.f * deltaTime;
    myTransform->SetRelativePosition(nextPos);
}

void CMonsterChaseState::Exit(CGameObject* entity)
{
#ifdef _DEBUG;
    std::cout << "Exiting Monster Chase State" << std::endl;
#endif
}