#pragma once
#include "State.h"
#include <unordered_map>

class CStateManager
{
public:
    CStateManager() : m_CurrentState(nullptr) {}
    ~CStateManager();

    void Update(CGameObject* entity, float deltaTime);

    void ChangeState(CGameObject* entity, EState_Type newState);
    void AddState(CState* state);
    void AddState(EState_Type state, CState* stateInstance);
    void SetTransition(EState_Type from, const std::string& event, EState_Type to);

    void HandleEvent(CGameObject* entity, const std::string& event);


    EState_Type GetCurrentStateType() const { return m_CurrentState ? m_CurrentState->GetStateType() : EState_Type::END; }

private:
    CState* m_CurrentState;
    std::unordered_map<EState_Type, std::unordered_map<std::string, EState_Type>> m_TransitionTable;
    std::unordered_map<EState_Type, CState*> m_States;
};