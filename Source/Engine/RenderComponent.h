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
    CSharedPtr<CMaterial> GetMaterial(UINT32 idx = 0) { return m_Materials[idx]; }

    void SetMesh(CSharedPtr<CMesh> mesh)                { m_Mesh = mesh; }
    void SetMaterial(CSharedPtr<CMaterial> material, UINT32 idx = 0);

public:
    virtual void Render() = 0;

protected:
    CSharedPtr<CMesh>       m_Mesh;
    std::vector<CSharedPtr<CMaterial>>   m_Materials;
};

