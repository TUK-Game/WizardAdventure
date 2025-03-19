#include "pch.h"
#include "PlayerIdleState.h"


void CPlayerIdleState::Enter(CGameObject* entity)
{
}

void CPlayerIdleState::Update(CGameObject* entity, float deltaTime)
{
    // 상태 변경 예시
    //if (/*이동 입력 감지*/)
    //{
    //    entity->GetStateMachine().ChangeState(entity, EState_Type::Run);
    //}
}

void CPlayerIdleState::Exit(CGameObject* entity)
{
}