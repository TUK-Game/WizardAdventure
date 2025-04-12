#pragma once
#include "SkillObject.h"

class CMeteors :
    public CSkillObject
{
public:
    CMeteors(Vec3 centerPos, int count, float interval);
    virtual void Update();
    virtual void FinalUpdate();
    void SpawnMeteor();


private:
    int m_SpawnedCount = 0;
    float m_ElapsedTime = 0.f;
    Vec3 m_CenterPos;
    int m_TotalCount;
    float m_Interval;
};

