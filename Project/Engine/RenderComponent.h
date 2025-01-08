#pragma once
#include "Component.h"
class CRenderComponent :
    public CComponent
{
public:
    CRenderComponent(EComponent_Type type);
    ~CRenderComponent();

public:
    virtual void Render() = 0;

private:
    // TODO: Mesh, Material
};

