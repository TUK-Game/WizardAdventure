#pragma once
#include "SkillObject.h"

class CFirePillar :
    public CSkillObject
{
public:
    CFirePillar();

    void SetDuration(float duration) { m_Duration = duration; }
    void SetBasePos(const Vec3& pos) { m_BasePos = pos; }

    virtual void Update();
    virtual void FinalUpdate();

private:
    float m_Duration = 6.f;
    float m_ElapsedTime = 0.f;
    float m_ShakeAmount = 0.2f;
    Vec3 m_BasePos;
    float m_ShakeFreqX;
    float m_ShakeFreqZ;
    float m_PhaseOffsetX = 0.f;
    float m_PhaseOffsetZ = 0.f;
};

