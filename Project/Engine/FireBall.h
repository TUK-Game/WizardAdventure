#pragma once
#include "GameObject.h"

class CFireBall :
    public CGameObject
{
public:
    CFireBall();

    void SetDirection(const Vec3& dir) { m_Direction = dir; }
    void SetSpeed(float speed) { m_Speed = speed; }

    virtual void Update();
    virtual void FinalUpdate();

private:
    Vec3 m_Direction;
    float m_Speed;
    float m_Duration = 0.3f;
    float m_ElapsedTime = 0.f;
};

