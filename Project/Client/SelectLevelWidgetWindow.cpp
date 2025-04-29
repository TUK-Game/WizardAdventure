#include "pch.h"
#include "SelectLevelWidgetWindow.h"
#include <Engine/ButtonWidget.h>
#include <Engine/MeshRenderer.h>
#include <Engine/AssetManager.h>
#include <Engine/Transform.h>
#include <Engine/LevelManager.h>
#include <Engine/NetworkManager.h>
#include <Engine/ServerSession.h>
#include "Level_1.h"

CSelectLevelWidgetWindow::CSelectLevelWidgetWindow()
{
}

CSelectLevelWidgetWindow::~CSelectLevelWidgetWindow()
{
}

bool CSelectLevelWidgetWindow::Init(CPlayer* player)
{
	CButtonWidget* widget = CreateWidget<CButtonWidget>(L"FireMage", player);
	widget->GetTransform()->SetRelativePosition(-0.658, -0.2f, 0.f);
	widget->GetTransform()->SetRelativeScale(0.55f, 1.3f, 1.f);
	widget->SetFunction([]() {
		CLevelManager::GetInst()->ChangeLevel(new CLevel_1);
		std::cout << "hi";
		});

	widget->SetButtonTexture(
		CAssetManager::GetInst()->FindAsset<CTexture>(L"FireMage"),
		CAssetManager::GetInst()->FindAsset<CTexture>(L"FireMage_Hover"),
		CAssetManager::GetInst()->FindAsset<CTexture>(L"FireMage_Pressed")
	);

	widget = CreateWidget<CButtonWidget>(L"IceMage", player);
	widget->GetTransform()->SetRelativePosition(-0.058, -0.2f, 0.f);
	widget->GetTransform()->SetRelativeScale(0.55f, 1.3f, 1.f);
	widget->SetFunction([]() {
		CLevelManager::GetInst()->ChangeLevel(new CLevel_1);
		});

	widget->SetButtonTexture(
		CAssetManager::GetInst()->FindAsset<CTexture>(L"IceMage"),
		CAssetManager::GetInst()->FindAsset<CTexture>(L"IceMage_Hover"),
		CAssetManager::GetInst()->FindAsset<CTexture>(L"IceMage_Pressed")
	);

	widget = CreateWidget<CButtonWidget>(L"LightningMage", player);
	widget->GetTransform()->SetRelativePosition(0.542, -0.2f, 0.f);
	widget->GetTransform()->SetRelativeScale(0.55f, 1.3f, 1.f);
	widget->SetFunction([]() {
		CLevelManager::GetInst()->ChangeLevel(new CLevel_1);
		});

	widget->SetButtonTexture(
		CAssetManager::GetInst()->FindAsset<CTexture>(L"LightningMage"),
		CAssetManager::GetInst()->FindAsset<CTexture>(L"LightningMage_Hover"),
		CAssetManager::GetInst()->FindAsset<CTexture>(L"LightningMage_Pressed")
	);

	return true;
}

void CSelectLevelWidgetWindow::Update()
{
	CWidgetWindow::Update();
}
