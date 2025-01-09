#pragma once

#include "Component.h"
#include "Mesh.h"

class CRenderComponent :
    public CComponent
{
public:
    CRenderComponent(EComponent_Type type);
    ~CRenderComponent();

public:
    CSharedPtr<CMesh> GetMesh() { return m_Mesh; }

    void SetMesh(CSharedPtr<CMesh> mesh)    { m_Mesh = mesh; }

public:
    virtual void Render() = 0;

private:
    CSharedPtr<CMesh>   m_Mesh;

    // TODO: Material
};

