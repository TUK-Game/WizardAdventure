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
	GetMaterial()->Binding();
	// TODO: GetMaterial()->Binding();

	// ������ ����
	GetMesh()->Render();
}
