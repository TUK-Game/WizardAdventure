#pragma once
#include "GameObject.h"

class CAttackRangeCircle : public CGameObject
{
public:
    CAttackRangeCircle();

    void SetDuration(float duration) { m_Duration = duration; }
    void SetScaleRange(const Vec3& start, const Vec3& end)
    {
        m_StartScale = start;
        m_EndScale = end;
    }

    Vec3 GetEndScale() const { return m_EndScale; }

    virtual void Update() override;
    virtual void FinalUpdate() override;

private:
    float m_Duration = 2.f;
    float m_ElapsedTime = 0.f;

    Vec3 m_StartScale = Vec3(0.1f, 0.1f, 0.1f);
    Vec3 m_EndScale = Vec3(1.f, 1.f, 1.f);
};