#pragma once

#include "Script.h"

class CCameraScript :
    public CScript
{
public:
    CCameraScript();
    ~CCameraScript();

public:
    virtual void Update() override;

private:
    void Move();

public:
    virtual CCameraScript* Clone() override { return new CCameraScript(*this); }

private:
    float   m_Speed;
};

