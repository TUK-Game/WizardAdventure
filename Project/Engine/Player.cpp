#include "pch.h"
#include "Player.h"
#include "StateManager.h"
#include "PlayerIdleState.h"
#include "PlayerRunState.h"
#include "PlayerDashState.h"
#include "PlayerAttackQState.h"
#include "PlayerAttackRState.h"
#include "PlayerAttackEState.h"
#include "PlayerAttackLState.h"
#include "Transform.h"
#include "Engine.h"
#include "SkillManager.h"
#include <iostream>
#include "Engine.h"

//#include <Engine/Engine.h>

CPlayer::CPlayer(EPlayerAttribute attribute)
    : m_Attribute(attribute), m_SkillManager(new CSkillManager(attribute, this))
{
    CreateStateManager();
}

CPlayer::~CPlayer()
{
    if (m_SkillManager)
        delete m_SkillManager;
}

void CPlayer::Begin()
{
    CGameObject::Begin();
    m_StateManager->ChangeState(this, EState_Type::Idle);
}

void CPlayer::Update()
{
    if (m_StateManager) {
        m_StateManager->Update(this, DELTA_TIME);
    }
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

void CPlayer::CreateStateManager()
{
    m_StateManager = new CStateManager();
    m_StateManager->AddState(new CPlayerIdleState);
    m_StateManager->AddState(new CPlayerRunState);
    m_StateManager->AddState(new CPlayerDashState);
    m_StateManager->AddState(new CPlayerAttackQState);
    m_StateManager->AddState(new CPlayerAttackRState);
    m_StateManager->AddState(new CPlayerAttackEState);
    m_StateManager->AddState(new CPlayerAttackEState);
    m_StateManager->AddState(new CPlayerAttackLState);

    m_StateManager->SetTransition(EState_Type::Idle, "Move", EState_Type::Run);
    m_StateManager->SetTransition(EState_Type::Idle, "Dash", EState_Type::Dash);
    m_StateManager->SetTransition(EState_Type::Idle, "Attack_Q", EState_Type::Attack_Q);
    m_StateManager->SetTransition(EState_Type::Idle, "Attack_R", EState_Type::Attack_R);
    m_StateManager->SetTransition(EState_Type::Idle, "Attack_E", EState_Type::Attack_E);
    m_StateManager->SetTransition(EState_Type::Idle, "Attack_LButton", EState_Type::Attack_LButton);


    m_StateManager->SetTransition(EState_Type::Run, "Stop", EState_Type::Idle);
    m_StateManager->SetTransition(EState_Type::Run, "Dash", EState_Type::Dash);
    m_StateManager->SetTransition(EState_Type::Run, "Attack_Q", EState_Type::Attack_Q);
    m_StateManager->SetTransition(EState_Type::Run, "Attack_R", EState_Type::Attack_R);
    m_StateManager->SetTransition(EState_Type::Run, "Attack_E", EState_Type::Attack_E);
    m_StateManager->SetTransition(EState_Type::Run, "Attack_LButton", EState_Type::Attack_LButton);

    m_StateManager->SetTransition(EState_Type::Dash, "EndDash", EState_Type::Run);

    m_StateManager->SetTransition(EState_Type::Attack_Q, "EndAttack", EState_Type::Idle);
    m_StateManager->SetTransition(EState_Type::Attack_R, "EndAttack", EState_Type::Idle);
    m_StateManager->SetTransition(EState_Type::Attack_E, "EndAttack", EState_Type::Idle);
    m_StateManager->SetTransition(EState_Type::Attack_LButton, "EndAttack", EState_Type::Idle);
}

void CPlayer::Move(Vec3 moveDir, bool shouldRotate)
{
    CTransform* transform = GetTransform();
    if (!transform) return;

    if (moveDir.Length() > 0)
    {
        moveDir.Normalize(); 
        m_currentMoveDir = moveDir;
        transform->SetRelativePosition(transform->GetRelativePosition() + (moveDir * m_Speed * CEngine::GetInst()->GetDeltaTime()));

        if (shouldRotate) {
            float angle = atan2(moveDir.x, moveDir.z) * (180.0f / XM_PI); 
            transform->SetRelativeRotation(0.f, angle + 180.f, 0.f);
        }
    }
}

void CPlayer::Attack(int skillIndex)
{
    m_SkillManager->UseSkill(skillIndex);
}

void CPlayer::CollisionBegin(CBaseCollider* src, CBaseCollider* dest)
{
    std::cout << "아야" << std::endl;
}
