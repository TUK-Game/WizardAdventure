#pragma once
#include "Script.h"
class CTestDragon :
    public CScript
{
public:
    virtual void Update() override;

public:
    virtual CTestDragon* Clone() override { return new CTestDragon(*this); }
};

