#pragma once
#include "SkillObject.h"

class CFireTower :
    public CSkillObject
{
public:
    CFireTower();

    void SetDuration(float duration) { m_Duration = duration; }

    virtual void Update();
    virtual void FinalUpdate();

private:
    float m_Duration = 6.f;
    float m_ElapsedTime = 0.f;
};

