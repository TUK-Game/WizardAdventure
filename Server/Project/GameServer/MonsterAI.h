#pragma once
#include "Monster.h"

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
    void UpdateType(EMonsterType type);
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
    float m_AttackDuration = 1.4f;
    float m_AttackSpawn = m_AttackDuration * 0.53;
    float m_AttackDelete = m_AttackDuration * 0.63;

    float m_DamagedTime = 0.f;
    float m_DamagedDuration = 0.6f;

    float m_DeathTime = 0.f;
    float m_DeathDuration = 3.f;

    float m_DissolveTime = 0.f;
    float m_DissolveDuration = 1.f;

    float m_SpawnTime = 0.f;
    float m_SpawnDuration = 3.5f;

    float m_AngleThresholdDeg = 20.f;

    float m_TargetAngle = 0.f;
    CPlayerRef m_Target = nullptr;
    CMonster* m_Owner = nullptr;

    bool m_bAttack{};

    std::vector<uint32> m_ProjectileIds;
};

