#pragma once
#include "GameObject.h"

class CFireSword :
    public CGameObject
{
public:
    CFireSword();

    void SetDirection(const Vec3& dir) { m_Direction = dir; m_Direction.Normalize(); }
    void SetReadyDirection(const Vec3& dir) { m_ReadyDirection = dir; m_ReadyDirection.Normalize(); }
    void SetTargetPos(const Vec3& pos) { m_TargetPos = pos; }

    void SetSpeed(float speed) { m_Speed = speed; }
    void SetWaitTimeForRotate(float time) { m_RotateWaitTime = time; }
    void SetWaitTimeForTranslate(float time) { m_TranslateWaitTime = time; }
    void SetReadyScale(float scale) { m_ReadyScale = scale; }
    virtual void Update();
    virtual void FinalUpdate();

private:
    Vec3 m_Direction;
    Vec3 m_ReadyDirection;
    Vec3 m_TargetPos;
    float m_Speed {600.f};
    bool m_ReadyToFire {false};
    bool m_ReadyToRotate {false};

    float m_ReadyScale {100.f};
    float m_RotateWaitTime{0.5f};
    float m_TranslateWaitTime{0.5f};
    float m_Elapsed {0.f};
};

