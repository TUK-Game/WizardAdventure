#pragma once

#include "Script.h"

class CCameraScript :
    public CScript
{
public:
    CCameraScript();
    ~CCameraScript();

public:
    virtual void Begin() override;
    virtual void Update() override;

private:
    void FreeMove();
    void FixedMove();

public:

    virtual CCameraScript* Clone() override { return new CCameraScript(*this); }

private:
    float                 m_Speed;
            
    class CTransform*     m_TargetTransform;
    Vec3                  m_Offset;
};

