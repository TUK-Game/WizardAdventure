#pragma once
#include "State.h"
#include <unordered_map>

class CStateManager
{
public:
	CStateManager() : m_CurrentState(nullptr) {}
	virtual ~CStateManager();

	virtual void Update(CGameObject* entity, float deltaTime);

	virtual void ChangeState(CGameObject* entity, EState_Type newState);
	void AddState(CState* state);
	void AddState(EState_Type state, CState* stateInstance);
	void SetTransition(EState_Type from, const std::string& event, EState_Type to);

	virtual void HandleEvent(CGameObject* entity, const std::string& event);


	EState_Type GetCurrentStateType() const { return m_CurrentState ? m_CurrentState->GetStateType() : EState_Type::END; }

protected:
	CState* m_CurrentState{ nullptr };
	std::unordered_map<EState_Type, std::unordered_map<std::string, EState_Type>> m_TransitionTable;
	std::unordered_map<EState_Type, CState*> m_States;
};