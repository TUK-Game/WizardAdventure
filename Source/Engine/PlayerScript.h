#pragma once
#include "Script.h"

class CPlayerScript :
    public CScript
{
public:
    virtual void Update() override;
    virtual CPlayerScript* Clone() override { return new CPlayerScript(*this); }
};

