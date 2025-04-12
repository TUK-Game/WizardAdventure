#pragma once
#include "SkillObject.h"

class CFireBall :
    public CSkillObject
{
public:
    CFireBall();

    void SetDirection(const Vec3& dir) { m_Direction = dir; }
    void SetSpeed(float speed) { m_Speed = speed; }
    void SetDuration(float duration) { m_Duration = duration; }

    virtual void Update();
    virtual void FinalUpdate();

    virtual void CollisionBegin(CBaseCollider* src, CBaseCollider* dest) override;

private:
    Vec3 m_Direction;
    float m_Speed;
    float m_Duration = 0.3f;
    float m_ElapsedTime = 0.f;
};

