#include "pch.h"
#include "Player.h"
#include "StateManager.h"
#include "PlayerIdleState.h"
#include <iostream>

CPlayer::CPlayer()
{
    m_StateManager = new CStateManager();
    m_StateManager->AddState(new CPlayerIdleState);
    m_StateManager->ChangeState(this, EState_Type::Idle);
}

CPlayer::~CPlayer()
{
    if (m_StateManager)
        delete m_StateManager;
}

void CPlayer::Begin()
{
    CGameObject::Begin();
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

//void CPlayer::ChangeState(PlayerState* newState)
//{
//    if (m_CurrentState)
//        delete m_CurrentState; // 기존 상태 삭제
//
//    m_CurrentState = newState;
//    m_CurrentState->Enter(this);
//}

//void CPlayer::Move(float deltaTime)
//{
//    CTransform* transform = GetTransform();
//    if (transform)
//    {
//        transform->Translate(Vector3(m_Speed * deltaTime, 0, 0)); // 가로 이동
//    }
//}

//void CPlayer::Jump()
//{
//    CRigidBody* rigidBody = GetRigidBody();
//    if (rigidBody)
//    {
//        rigidBody->ApplyForce(Vector3(0, m_JumpForce, 0)); // 점프 적용
//    }
//}

void CPlayer::Attack()
{
    std::cout << "Player is attacking!" << std::endl;
}