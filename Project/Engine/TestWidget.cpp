#include "pch.h"
#include "TestWidget.h"
#include "ImageWidget.h"
#include "MeshRenderer.h"
#include "Transform.h"
#include "AssetManager.h"

TestWidget::TestWidget()
{
}

TestWidget::~TestWidget()
{
}

bool TestWidget::Init()
{
	CWidgetWindow::Init();
	CImageWidget* widget = CreateWidget<CImageWidget>(L"image");
	widget->AddComponent(new CMeshRenderer);
	widget->AddComponent(new CTransform);
	widget->SetTexture(L"Kita");
	widget->GetMeshRenderer()->SetMesh(CAssetManager::GetInst()->FindAsset<CMesh>(L"Rectangle"));
	widget->GetTransform()->SetRelativeScale(10, 10, 10);
	widget->SetEnable(false);

	widget = CreateWidget<CImageWidget>(L"MiniMap");
	widget->AddComponent(new CMeshRenderer);
	widget->AddComponent(new CTransform);
	widget->SetTexture(L"MiniMapTarget");
	widget->GetMeshRenderer()->SetMesh(CAssetManager::GetInst()->FindAsset<CMesh>(L"Rectangle"));
	//widget->GetTransform()->SetRelativeScale(190, 190, 10);
	widget->GetTransform()->SetRelativePosition(1, 0, 0);
	widget->SetEnable(false);
	return true;
}


