#pragma once
#include "Component.h"
class CMonsterAI :
    public CComponent
{
public:
    CMonsterAI();
    virtual ~CMonsterAI() = default;

public:
    virtual void Begin() override;
    virtual void Update() override;
    virtual void FinalUpdate() override;

    virtual CMonsterAI* Clone() override { return new CMonsterAI(*this); }

    CGameObject* GetTarget()            { return m_Target; }
    float GetTargetAngle() const { return m_TargetAngle; }
    void RotateToTarget(float deltaTime);
    bool IsFacingTarget(float angleThresholdDeg = 20.f);
private:
    float m_DetectRange = 5000.f;
    float m_AttackRange = 500.f;

    float m_TargetAngle = 0.f;
    CGameObject* m_Target = nullptr;
private:
    void DetectTarget();
    void UpdateAI();
};

