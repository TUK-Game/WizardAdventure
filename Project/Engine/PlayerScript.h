#pragma once
#include "Script.h"

class CPlayerScript :
    public CScript
{
public:
    virtual void Update() override;
    virtual CPlayerScript* Clone() override { return new CPlayerScript(*this); }

private:
    void InitBeforeDeath(class CPlayer* player);
    void KeyInputBeforeDeath(class CPlayer* player);

private:
    int32 m_BeforDeath_TargetId{ -1 };
};

