#pragma once

#include "Component.h"

class CUI : public CComponent
{
public:
    CUI();
    virtual ~CUI();

public:
    virtual void Begin() override;
    virtual void Update() override;
    virtual void FinalUpdate() override;
    virtual CUI* Clone() override { return new CUI(*this); }


};
