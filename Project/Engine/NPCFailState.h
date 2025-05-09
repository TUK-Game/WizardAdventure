#pragma once
#include "State.h"
class CNPCFailState :
    public CState
{
    void Enter(CGameObject* entity) override;
    void Update(CGameObject* entity, float deltaTime) override;
    void Exit(CGameObject* entity) override;
    EState_Type GetStateType() const override { return EState_Type::Death; }

private:
    float m_ElapsedTime{};
    float m_Duration{};
};

