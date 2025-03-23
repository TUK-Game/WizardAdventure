#include "pch.h"
#include "MeshRenderer.h"
#include "Transform.h"
#include "AssetManager.h"
#include "GraphicShader.h"	// Temp
#include "InstancingBuffer.h"
#include "Animator.h"

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
	// ������ ����
	for (UINT32 i = 0; i < m_Materials.size(); ++i)
	{
		CMaterial* material = m_Materials[i];

		if (material == nullptr || material->GetGraphicsShader() == nullptr)
			continue;

		GetTransform()->GraphicsBinding();

		if (GetAnimator())
		{
			GetAnimator()->PushData();
			material->SetInt(1, 1);
		}

		GetMaterial()->GraphicsBinding();
		GetMesh()->Render(1, i);
	}
}

void CMeshRenderer::Render(std::shared_ptr<CInstancingBuffer>& buffer)
{
	for (UINT32 i = 0; i < m_Materials.size(); ++i)
	{
		CMaterial* material = m_Materials[i];

		if (material == nullptr || material->GetGraphicsShader() == nullptr)
			continue;

		buffer->PushData();

		if (GetAnimator())
		{
			GetAnimator()->PushData();
			material->SetInt(1, 1);
		}

		material->GraphicsBinding();
		m_Mesh->Render(buffer, i);
	}
}

void CMeshRenderer::RenderShadow()
{
	GetTransform()->GraphicsBinding();
	//������ material 
	CAssetManager::GetInst()->FindAsset<CMaterial>(L"Shadow")->GraphicsBinding();
	m_Mesh->Render();
}

void CMeshRenderer::RenderMap(std::shared_ptr<CInstancingBuffer>& buffer)
{	
	CAssetManager::GetInst()->FindAsset<CMaterial>(L"DeferredMap")->GraphicsBinding();
	buffer->PushData();
	m_Mesh->Render(buffer, 0);
}

void CMeshRenderer::RenderMap()
{
	GetTransform()->GraphicsBinding();
	CMaterial* material = CAssetManager::GetInst()->FindAsset<CMaterial>(L"DeferredMap");
	material->SetTexture(0, GetMaterial()->GetTexture(0));
	material->GraphicsBinding();
	m_Mesh->Render();
}

UINT64 CMeshRenderer::GetInstanceID()
{
	if (m_Mesh == nullptr || m_Materials[0] == nullptr)
		return 0;

	InstanceID instanceID{ m_Mesh->GetID(), m_Materials[0]->GetID()};
	return instanceID.id;
}
