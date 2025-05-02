#pragma once
#include "State.h"
class CMonsterDeathState :
    public CState
{
    void Enter(CGameObject* entity) override;
    void Update(CGameObject* entity, float deltaTime) override;
    void Exit(CGameObject* entity) override;
    EState_Type GetStateType() const override { return EState_Type::Death; }

private:
    float m_DeathDuration = 2.0f;
    float m_ElapsedTime = 0.f;
};

