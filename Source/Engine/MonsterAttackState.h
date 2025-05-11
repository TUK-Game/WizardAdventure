#pragma once
#include "State.h"

class CMonsterAttackState :
    public CState
{
    void Enter(CGameObject* entity) override;
    void Update(CGameObject* entity, float deltaTime) override;
    void Exit(CGameObject* entity) override;
    EState_Type GetStateType() const override { return EState_Type::Attack; }

private:
    float m_AttackDuration = 2.0f;
    float m_AttackTime;
    float m_ElapsedTime = 0.f;
    bool m_bDoAttack = false;

    // temp
    EMonsterType m_MonsterType;
};

