#include "pch.h"
#include "TestWidget.h"
#include "ImageWidget.h"
#include "MeshRenderer.h"
#include "Transform.h"
#include "AssetManager.h"
#include "MapPlayerWidget.h"
#include "Player.h"

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

	//CMapPlayerWidget* pwidget = CreateWidget<CMapPlayerWidget>(L"PI");

	return true;
}

void TestWidget::AddPlayer(CPlayer* player, int idx)
{
	CMapPlayerWidget* pwidget = CreateWidget<CMapPlayerWidget>(L"PI" + idx);
	pwidget->InitPlayer(player, idx);
}


