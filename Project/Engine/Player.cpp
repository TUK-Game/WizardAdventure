#include "pch.h"
#include "Player.h"
#include "StateManager.h"
#include "PlayerIdleState.h"
#include "PlayerRunState.h"
#include "Transform.h"
#include <iostream>

CPlayer::CPlayer()
{
    m_StateManager = new CStateManager();
    m_StateManager->AddState(new CPlayerIdleState);
    m_StateManager->AddState(new CPlayerRunState);
    m_StateManager->SetTransition(EState_Type::Idle, "Move", EState_Type::Run);
    m_StateManager->SetTransition(EState_Type::Run, "Stop", EState_Type::Idle);
}

CPlayer::~CPlayer()
{
}

void CPlayer::Begin()
{
    CGameObject::Begin();
    m_StateManager->ChangeState(this, EState_Type::Idle);
}

void CPlayer::Update()
{
    if (m_StateManager)
        m_StateManager->Update(this, 0.016f);
    CGameObject::Update();
}

void CPlayer::FinalUpdate()
{
    CGameObject::FinalUpdate();
}

void CPlayer::Render()
{
    CGameObject::Render();
}

void CPlayer::Move(Vec3 moveDir)
{
    CTransform* transform = GetTransform();
    if (!transform) return;

    if (moveDir.Length() > 0)
    {
        moveDir.Normalize(); 
        m_currentMoveDir = moveDir;
        transform->SetRelativePosition(transform->GetRelativePosition() + moveDir);
        float angle = atan2(moveDir.x, moveDir.z) * (180.0f / XM_PI); // 라디안 → 도 단위 변환
        transform->SetRelativeRotation(0.f, angle + 180.f, 0.f);
    }
}

void CPlayer::Attack()
{
    std::cout << "Player is attacking!" << std::endl;
}