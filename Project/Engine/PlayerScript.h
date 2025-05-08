#pragma once
#include "Script.h"

class CPlayerScript :
    public CScript
{
public:
    virtual void Update() override;
    virtual CPlayerScript* Clone() override { return new CPlayerScript(*this); }

private:
    int32 m_BeforDeath_TargetId{ -1 };
};

