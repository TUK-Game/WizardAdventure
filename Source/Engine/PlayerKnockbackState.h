#pragma once
#include "State.h"

class CPlayerKnockbackState : public CState
{
public:
    void Enter(CGameObject* entity) override;
    void Update(CGameObject* entity, float deltaTime) override;
    void Exit(CGameObject* entity) override;
    EState_Type GetStateType() const override { return EState_Type::Knockback; }

private:
    float m_Elapsed = 0.f;
    float m_Duration = 0.5f; // 넉백 지속 시간
    Vec3 m_KnockbackDir;
};