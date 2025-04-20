#pragma once

class CMonsterAI
{
public:
    CMonsterAI();
    ~CMonsterAI();

public:
    void SetOwner(CMonster* monster) { m_Owner = monster; }
public:
    virtual void Update(float deltaTime);

    CPlayerRef GetTarget() { return m_Target; }
    float GetTargetAngle() const { return m_TargetAngle; }
    void RotateToTarget(float deltaTime);
    bool IsFacingTarget(float angleThresholdDeg = 20.f);
private:
    void DetectTarget();
    void UpdateAI(float deltaTime);
    void Chase(float deltaTime);
    void HandleMoveMonster(float deltaTime);
    void HandleGravityMonster(float deltaTime);
private:
    float m_DetectRange = 5000.f;
    float m_AttackRange = 500.f;

    float m_AttackTime = 0.f;
    float m_AttackDuration = 1.5f;

    float m_TargetAngle = 0.f;
    CPlayerRef m_Target = nullptr;
    CMonster* m_Owner = nullptr;
};

