#pragma once
#include "State.h"

class CPlayerDashState :
    public CState
{
    void Enter(CGameObject* entity) override;
    void Update(CGameObject* entity, float deltaTime) override;
    void Exit(CGameObject* entity) override;
    EState_Type GetStateType() const override { return EState_Type::Dash; }
    
private:
    float m_DashSpeed = 1000.f;
    float m_DashDuration = 0.2f;
    float m_ElapsedTime = 0.f;
};

