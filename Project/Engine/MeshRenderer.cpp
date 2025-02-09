#include "pch.h"
#include "MeshRenderer.h"
#include "Transform.h"
#include "AssetManager.h"
#include "GraphicShader.h"	// Temp
#include "InstancingBuffer.h"

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

void CMeshRenderer::Render(std::shared_ptr<CInstancingBuffer>& buffer)
{
	buffer->PushData();
	m_Materials[0]->GraphicsBinding();
	m_Mesh->Render(buffer);
}

void CMeshRenderer::RenderShadow()
{
	GetTransform()->GraphicsBinding();
	//Ω¶µµøÏ material 
	CAssetManager::GetInst()->FindAsset<CMaterial>(L"Shadow")->GraphicsBinding();
	m_Mesh->Render();
}

UINT64 CMeshRenderer::GetInstanceID()
{
	if (m_Mesh == nullptr || m_Materials[0] == nullptr)
		return 0;

	InstanceID instanceID{ m_Mesh->GetID(), m_Materials[0]->GetID()};
	return instanceID.id;
}
