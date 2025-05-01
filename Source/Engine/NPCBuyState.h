#include "State.h"
#pragma once
class CNPCBuyState :
    public CState
{
    void Enter(CGameObject* entity) override;
    void Update(CGameObject* entity, float deltaTime) override;
    void Exit(CGameObject* entity) override;
    EState_Type GetStateType() const override { return EState_Type::Buy; }

private:
    float m_ElapsedTime{};
    float m_Duration{};
};


