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

    virtual void SetTarget(const Vec3& pos) { m_Interpolator->SetTarget(pos); }
    void SetStat(const MonsterStat& stat);
    const MonsterStat& GetStat() const { return m_Stat; }

    EMonsterType GetType() const { return m_Type; }

    void CreateStateManager();

private:
    MonsterStat m_Stat;
    EMonsterType m_Type;
    CInterpolator* m_Interpolator;
};

