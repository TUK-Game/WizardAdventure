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
    {
        std::cout << "초기기기ㅣ" << std::endl;
        m_CurrentState->Exit(entity);
    }

    m_CurrentState = m_States[newState];
    if (m_CurrentState)
        m_CurrentState->Enter(entity);
}

void CStateManager::AddState(CState* state)
{
    m_States[state->GetStateType()] = state;
}

void CStateManager::AddState(EState_Type state, CState* stateInstance)
{
    m_States[state] = stateInstance;
}


void CStateManager::HandleEvent(CGameObject* owner, const std::string& event)
{
    if (m_CurrentState)
    {
        EState_Type currentType = m_CurrentState->GetStateType();
        if (m_TransitionTable[currentType].find(event) != m_TransitionTable[currentType].end())
        {
            EState_Type nextType = m_TransitionTable[currentType][event];
            ChangeState(owner, nextType);
        }
    }
}

void CStateManager::SetTransition(EState_Type from, const std::string& event, EState_Type to)
{
    m_TransitionTable[from][event] = to;
}