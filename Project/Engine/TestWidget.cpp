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
	widget->SetTexture(L"MiniMapTarget");
	widget->GetTransform()->SetRelativeScale(2, 2, 2);

	//CMapPlayerWidget* pwidget = CreateWidget<CMapPlayerWidget>(L"PI");

	return true;
}

void TestWidget::AddPlayer(CPlayer* player, int idx)
{
	CMapPlayerWidget* pwidget = CreateWidget<CMapPlayerWidget>(L"PI" + idx);
	pwidget->InitPlayer(player, idx);
}


