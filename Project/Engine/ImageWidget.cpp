#include "pch.h"
#include "ImageWidget.h"
#include "AssetManager.h"
#include "RenderComponent.h"
#include "MeshRenderer.h"
#include "Transform.h"

CImageWidget::CImageWidget()
{

}

CImageWidget::~CImageWidget()
{
}

void CImageWidget::SetTexture(const std::wstring& name)
{
	CMaterial* material = new CMaterial;
	CTexture* texture = CAssetManager::GetInst()->FindAsset<CTexture>(name);

	CGraphicShader* shader = CAssetManager::GetInst()->FindAsset<CGraphicShader>(L"Texture");
	material->SetTexture(0, texture);
	material->SetGraphicsShader(shader);

	GetMeshRenderer()->SetMaterial(material);
}

bool CImageWidget::Init(CPlayer* player)
{
	CWidget::Init(player);
	AddComponent(new CMeshRenderer);
	AddComponent(new CTransform);	
	GetMeshRenderer()->SetMesh(CAssetManager::GetInst()->FindAsset<CMesh>(L"Rectangle"));

	return true;
}
