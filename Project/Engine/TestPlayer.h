#pragma once
#include "Script.h"
class CTestPlayer :
    public CScript
{
public:

    virtual void Update() override;

public:
    virtual CTestPlayer* Clone() override { return new CTestPlayer(*this); }

    int m_Prev = 0;
    float m_Speed = 300.f;
};

