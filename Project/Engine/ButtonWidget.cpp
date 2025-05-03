#include "pch.h"
#include "ButtonWidget.h"
#include "MeshRenderer.h"
#include "Transform.h"
#include "AssetManager.h"
#include "UIButton.h"

bool CButtonWidget::Init(CPlayer* none)
{
	AddComponent(new CMeshRenderer);
	AddComponent(new CTransform);
	AddComponent(new CUIButton);
	GetMeshRenderer()->SetMesh(CAssetManager::GetInst()->FindAsset<CMesh>(L"Rectangle"));
	CMaterial* material = new CMaterial;
	CGraphicShader* shader = CAssetManager::GetInst()->FindAsset<CGraphicShader>(L"Texture");
	material->SetGraphicsShader(shader);
	GetMeshRenderer()->SetMaterial(material);
	return true;
}

void CButtonWidget::Update()
{
	CImageWidget::Update();
}

void CButtonWidget::SetOutHover(std::function<void()> func)
{
	GetUIButton()->SetOutHover(func);
}

void CButtonWidget::SetOnHover(std::function<void()> func)
{
	GetUIButton()->SetOnHover(func);
}

void CButtonWidget::SetOnClick(std::function<void()> func)
{
	GetUIButton()->SetOnClick(func);
}

void CButtonWidget::SetButtonTexture(CTexture* defaultTex, CTexture* hoverTex, CTexture* pressedTex)
{
	GetUIButton()->SetBTNTextures(defaultTex, hoverTex, pressedTex);
}

CButtonWidget::CButtonWidget()
{
}

CButtonWidget::~CButtonWidget()
{
}
