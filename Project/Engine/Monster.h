#pragma once
#include "GameObject.h"

class CMonster : public CGameObject
{
public:
    CMonster();
    virtual ~CMonster();

    virtual void Begin();
    virtual void Update();
    virtual void FinalUpdate();
    virtual void Render();

    virtual void SetTarget(const Vec3& pos, const Vec3& rot) { m_Interpolator->SetTarget(pos, rot); }
    void SetStats(int maxHp, int hp) { m_Stats->maxHp = maxHp; m_Stats->currentHp = hp; }
    Stats* GetStat() { return m_Stats; }

    EMonsterType GetType() const { return m_Type; }

    void CreateStateManager();

public:
    void ReceiveDamage(int damage);
    void OnDeath();

protected:
    EMonsterType m_Type;
    Stats* m_Stats;
    CInterpolator* m_Interpolator;
};

