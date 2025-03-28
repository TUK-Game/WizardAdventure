#include "pch.h"
#include "TestWidget.h"
#include "ImageWidget.h"
#include "MeshRenderer.h"
#include "Transform.h"
#include "AssetManager.h"
#include "MapPlayerWidget.h"

TestWidget::TestWidget()
{
}

TestWidget::~TestWidget()
{
}

bool TestWidget::Init()
{
	CWidgetWindow::Init();

	CImageWidget* widget = CreateWidget<CImageWidget>(L"MiniMap");
	widget->AddComponent(new CMeshRenderer);
	widget->AddComponent(new CTransform);
	widget->SetTexture(L"MiniMapTarget");
	widget->GetMeshRenderer()->SetMesh(CAssetManager::GetInst()->FindAsset<CMesh>(L"Rectangle"));
	widget->GetTransform()->SetRelativeScale(2, 2, 2);
	widget->SetEnable(false);

	//CMapPlayerWidget* pwidget = CreateWidget<CMapPlayerWidget>(L"PI");


	return true;
}


