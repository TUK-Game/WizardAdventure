#include "pch.h"
#include "StateManager.h"
#include "GameObject.h"

CStateManager::~CStateManager()
{
    for (auto& state : m_States)
        delete state.second;
}

void CStateManager::Update(CGameObject* entity, float deltaTime)
{
    if (m_CurrentState)
        m_CurrentState->Update(entity, deltaTime);
}

void CStateManager::ChangeState(CGameObject* entity, EState_Type newState)
{
    if (m_CurrentState)
        m_CurrentState->Exit(entity);

    m_CurrentState = m_States[newState];
    if (m_CurrentState)
        m_CurrentState->Enter(entity);
}

void CStateManager::AddState(CState* state)
{
    m_States[state->GetStateType()] = state;
}