#pragma once

#include "Component.h"
#include "Mesh.h"
#include "Material.h"

class CRenderComponent :
    public CComponent
{
public:
    CRenderComponent(EComponent_Type type);
    ~CRenderComponent();

public:
    CSharedPtr<CMesh> GetMesh()         { return m_Mesh; }
    CSharedPtr<CMaterial> GetMaterial() { return m_Material; }

    void SetMesh(CSharedPtr<CMesh> mesh)                { m_Mesh = mesh; }
    void SetMaterial(CSharedPtr<CMaterial> material)    { m_Material = material; }

public:
    virtual void Render() = 0;

private:
    CSharedPtr<CMesh>       m_Mesh;
    CSharedPtr<CMaterial>   m_Material;
};

