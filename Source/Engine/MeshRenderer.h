#pragma once

#include "RenderComponent.h"

class CMeshRenderer :
	public CRenderComponent
{
public:
    CMeshRenderer();
    ~CMeshRenderer();

public:
    virtual void FinalUpdate() override;
    virtual void Render() override;

public:
    virtual CMeshRenderer* Clone() override { return new CMeshRenderer(*this); }
};

