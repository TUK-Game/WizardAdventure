#pragma once
#include "SkillObject.h"
#include "AnimatedBillboardEffect.h"

class CFireTower :
    public CSkillObject
{
public:
    CFireTower();

    void SetDuration(float duration) { m_Duration = duration; }
    void SetScaleDuration(float duration) { m_ScaleDuration = duration; }
    
    void UpdateScaleLerp();

    virtual void Update();
    virtual void FinalUpdate();

private:
    float m_Duration = 6.f;
    float m_ScaleDuration = 6.f;
    float m_ElapsedTime = 0.f;
    CAnimatedBillboardEffect* m_FireEffect = nullptr;
    bool m_bFinishScale = false;
    
};

