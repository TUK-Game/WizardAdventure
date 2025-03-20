#include "pch.h"
#include "PlayerRunState.h"


void CPlayerRunState::Enter(CGameObject* entity)
{
}

void CPlayerRunState::Update(CGameObject* entity, float deltaTime)
{
    // 상태 변경 예시
    //if (/*이동 입력 감지*/)
    //{
    //    entity->GetStateMachine().ChangeState(entity, EState_Type::Run);
    //}
}

void CPlayerRunState::Exit(CGameObject* entity)
{
}