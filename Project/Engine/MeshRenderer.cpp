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
	// 위치정보 업데이트
	GetTransform()->Binding();

	// 사용할 쉐이더 바인딩
	// Temp
	GetMaterial()->Binding();
	// TODO: GetMaterial()->Binding();

	// 렌더링 시작
	GetMesh()->Render();
}
