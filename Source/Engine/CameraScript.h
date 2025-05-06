#pragma once

#include "Script.h"

class CCameraScript :
    public CScript
{
public:
    CCameraScript();
    ~CCameraScript();

public:
    void SetTransform(class CTransform* transform) { m_TargetTransform = transform; }
    void SetInteractionZoomTarget(const Vec3& pos) { m_InteractionZoomTarget = pos; }
    void SetInteractionStartPos(const Vec3& pos) { m_InteractionStartPos = pos; }
    void SetInteractionDir(const Vec3& start, const Vec3& target) { m_InteractionStartDir = start; m_InteractionZoomDir = target; }
    float LerpAngle(float start, float end, float t);
    Vec3 LerpAngles(const Vec3& start, const Vec3& end, float t);
public:
    virtual void Begin() override;
    virtual void Update() override;

private:
    void FreeMove();
    void FixedMove();
    void UpdateDirectionalLight();

    void MoveToTarget(bool isLayerCheck, float maxHeight = 0.f, EWIDGETWINDOW_TYPE windowtype = EWIDGETWINDOW_TYPE::END);
    void RollBackCamera();
public:

    virtual CCameraScript* Clone() override { return new CCameraScript(*this); }

private:
    float                 m_Speed;
            
    class CTransform*     m_TargetTransform;
    Vec3                  m_Offset;
    Vec3                  m_InteractionZoomTarget;
    Vec3                  m_InteractionStartPos;
    Vec3                  m_InteractionStartDir;
    Vec3                  m_InteractionZoomDir;
    float                 m_MoveDuration{ 1.f };
    float                 m_ElapsedTime{};
};

