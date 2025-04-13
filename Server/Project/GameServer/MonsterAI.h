#pragma once

class CMonsterAI
{
public:
    CMonsterAI();
    ~CMonsterAI();

public:
    virtual void Update();

    CPlayerRef GetTarget() { return m_Target; }
    float GetTargetAngle() const { return m_TargetAngle; }
    void RotateToTarget(float deltaTime);
    bool IsFacingTarget(float angleThresholdDeg = 20.f);
private:
    void DetectTarget();
    void UpdateAI();

private:
    float m_DetectRange = 5000.f;
    float m_AttackRange = 500.f;

    float m_TargetAngle = 0.f;
    CPlayerRef m_Target = nullptr;
    CMonsterRef m_Owner = nullptr;
};

