#pragma once
#include "GameObject.h"

class CAttackRangeRect : public CGameObject
{
public:
    CAttackRangeRect();

    void SetDuration(float duration) { m_Duration = duration; }
    void SetScaleRange(const Vec3& start, const Vec3& end) {
        m_StartScale = start;
        m_EndScale = end;
    }
    void SetInitialPosition(const Vec3& pos) { m_InitialPosition = pos; }

    virtual void Update() override;
    virtual void FinalUpdate() override;

private:
    float m_Duration;
    float m_ElapsedTime = 0.f;

    Vec3 m_StartScale;
    Vec3 m_EndScale; 
    Vec3 m_InitialPosition;

};