#include "pch.h"
#include "MeshRenderer.h"
#include "Transform.h"
#include "AssetManager.h"
#include "GraphicShader.h"	// Temp

CMeshRenderer::CMeshRenderer()
	: CRenderComponent(EComponent_Type::MeshRenderer)
{
}

CMeshRenderer::~CMeshRenderer()
{
}

void CMeshRenderer::FinalUpdate()
{
}

void CMeshRenderer::Render()
{
	// ∑ª¥ı∏µ Ω√¿€
	for (UINT32 i = 0; i < m_Materials.size(); ++i)
	{
		CMaterial* material = m_Materials[i];

		if (material == nullptr || material->GetGraphicsShader() == nullptr)
			continue;

		GetTransform()->GraphicsBinding();
		GetMaterial()->GraphicsBinding();
		GetMesh()->Render(1, i);
	}
}
