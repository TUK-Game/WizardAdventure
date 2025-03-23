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
private:
    float m_DetectRange = 5000.f;
    float m_AttackRange = 500.f;

    CGameObject* m_Target = nullptr;
private:
    void DetectTarget();
    void UpdateAI();
};

