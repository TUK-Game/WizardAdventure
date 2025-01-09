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
	// ��ġ���� ������Ʈ
	GetTransform()->Binding();

	// ����� ���̴� ���ε�
	// Temp
	CSharedPtr<CGraphicShader> shader = CAssetManager::GetInst()->FindAsset<CGraphicShader>(L"default");
	shader->Update();
	// TODO: GetMaterial()->Binding();

	// ������ ����
	GetMesh()->Render();
}
