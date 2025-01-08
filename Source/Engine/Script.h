#pragma once

#include "Component.h"

class CScript :
    public CComponent
{
public:
    CScript();
    ~CScript();

public:
    virtual void Update() = 0;
    virtual void FinalUpdate() final {}

public:
    virtual CScript* Clone() = 0;
};

