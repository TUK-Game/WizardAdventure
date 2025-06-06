#include "pch.h"
#include "StartLevelWidgetWindow.h"
#include <Engine/ButtonWidget.h>
#include <Engine/MeshRenderer.h>
#include <Engine/AssetManager.h>
#include <Engine/Transform.h>
#include <Engine/LevelManager.h>
#include "Level_Select.h"
#include "Level_1.h"

CStartLevelWidgetWindow::CStartLevelWidgetWindow()
{
}

CStartLevelWidgetWindow::~CStartLevelWidgetWindow()
{
}

bool CStartLevelWidgetWindow::Init(CPlayer* player)
{
	CButtonWidget* widget = CreateWidget<CButtonWidget>(L"StartBTN", player);
	widget->GetTransform()->SetRelativePosition(0.75f, -0.25f, 0.f);
	widget->GetTransform()->SetRelativeScale(0.25f, 0.2f, 1.f);
	widget->SetOnClick([]() {
#ifdef AUTO_SERVER_CONNECT
		CLevelManager::GetInst()->ChangeLevel(new CLevel_Select);
#else
		CLevelManager::GetInst()->ChangeLevel(new CLevel_1);
#endif
		});

	widget->SetButtonTexture(
		CAssetManager::GetInst()->FindAsset<CTexture>(L"StartBTN"),
		CAssetManager::GetInst()->FindAsset<CTexture>(L"StartBTN_Hover"),
		CAssetManager::GetInst()->FindAsset<CTexture>(L"StartBTN_Pressed")
	);

	widget = CreateWidget<CButtonWidget>(L"MenuBTN", player);
	widget->GetTransform()->SetRelativePosition(0.75f, -0.5f, 0.f);
	widget->GetTransform()->SetRelativeScale(0.25f, 0.2f, 1.f);
	widget->SetOnClick([]() {
		std::cout << "�� �޴���\n";
		});

	widget->SetButtonTexture(
		CAssetManager::GetInst()->FindAsset<CTexture>(L"MenuBTN"),
		CAssetManager::GetInst()->FindAsset<CTexture>(L"MenuBTN_Hover"),
		CAssetManager::GetInst()->FindAsset<CTexture>(L"MenuBTN_Pressed")
	);

	widget = CreateWidget<CButtonWidget>(L"ExitBTN", player);
	widget->GetTransform()->SetRelativePosition(0.75f, -0.75f, 0.f);
	widget->GetTransform()->SetRelativeScale(0.25f, 0.2f, 1.f);
	widget->SetOnClick([]() {
		PostQuitMessage(0);
		});

	widget->SetButtonTexture(
		CAssetManager::GetInst()->FindAsset<CTexture>(L"ExitBTN"),
		CAssetManager::GetInst()->FindAsset<CTexture>(L"ExitBTN_Hover"),
		CAssetManager::GetInst()->FindAsset<CTexture>(L"ExitBTN_Pressed")
	);

	return true;
}
