#pragma once
#include "SkillObject.h"


class CAdcBall :
    public CSkillObject
{
public:
    CAdcBall();

    void SetDuration(float duration) { m_Duration = duration; }
    virtual void Update();
    virtual void FinalUpdate();

    virtual void CollisionBegin(CBaseCollider* src, CBaseCollider* dest) override;
    virtual void OffParticles();

private:
    int m_LightParticleId = -1;
    float m_Duration = 1.f;
    float m_ElapsedTime = 0.f;
};

