#include "pch.h"
#include "Monster.h"
#include "StateManager.h"
#include "MonsterIdleState.h"
#include "MonsterChaseState.h"
#include "Transform.h"
#include "Engine.h"
#include "MonsterAI.h"
#include "SkillManager.h"
#include <iostream>

CMonster::CMonster()
{
    CreateStateManager();
    AddComponent(new CMonsterAI);
}

CMonster::~CMonster()
{
}

void CMonster::Begin()
{
    CGameObject::Begin();
}

void CMonster::Update()
{
    if (m_StateManager) {
        m_StateManager->Update(this, DELTA_TIME);
    }
    CGameObject::Update();
}

void CMonster::FinalUpdate()
{
    CGameObject::FinalUpdate();
}

void CMonster::Render()
{
    CGameObject::Render();
}

void CMonster::CreateStateManager()
{
    m_StateManager = new CStateManager();
    m_StateManager->AddState(new CMonsterIdleState);
    m_StateManager->AddState(new CMonsterChaseState);
    // Attack Ãß°¡
    m_StateManager->ChangeState(this, EState_Type::Idle);

    m_StateManager->SetTransition(EState_Type::Idle, "Chase", EState_Type::Chase);

    //m_StateManager->SetTransition(EState_Type::Chase, "Attack", EState_Type::Attack);
    m_StateManager->SetTransition(EState_Type::Chase, "Idle", EState_Type::Idle);

    //m_StateManager->SetTransition(EState_Type::Attack, "Idle", EState_Type::Idle);

}
