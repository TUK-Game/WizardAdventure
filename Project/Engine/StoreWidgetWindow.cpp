#include "pch.h"
#include "StoreWidgetWindow.h"
#include "ItemButtonWidget.h"
#include "ImageWidget.h"
#include "Transform.h"
#include "RenderManager.h"
#include "Camera.h"
#include "AssetManager.h"
#include "NetworkManager.h"
#include "ServerSession.h"
#include "TextWidget.h"
#include "Item.h"

CStoreWidgetWindow::CStoreWidgetWindow()
{
}

CStoreWidgetWindow::~CStoreWidgetWindow()
{
}

bool CStoreWidgetWindow::Init(CPlayer* player)
{
	CWidgetWindow::Init(player);

	CImageWidget* backgroundWidget = CreateWidget<CImageWidget>(L"backgroundWidget", player);
	backgroundWidget->SetTexture(L"ShopBackGround");
	backgroundWidget->GetTransform()->SetRelativePosition(-0.4f, 0.f, 0.f);
	backgroundWidget->GetTransform()->SetRelativeScale(1.2f, 2.f, 0.2f);

	backgroundWidget = CreateWidget<CImageWidget>(L"Popup", player);
	backgroundWidget->SetTexture(L"Popup");
	backgroundWidget->GetTransform()->SetRelativePosition(0.55f, -0.5f, 0.f);
	backgroundWidget->GetTransform()->SetRelativeScale(0.8f, 0.8f, 0.2f);

	CTextWidget* name = CreateWidget<CTextWidget>(L"Text", player);
	name->GetTransform()->SetRelativePosition(-0.2f, -0.8f, 0.f);
	//name->GetTransform()->SetRelativeScale(0.5f, 0.5f, 0.5f);
	name->SetBasicInfo(L"", L"±Ã¼­Ã¼", L"Yellow", Vec2(0.f, 0.f), Vec2(1000.f, 100.f));
	name->SetEnable(false);

	CItemButtonWidget* widget = CreateWidget<CItemButtonWidget>(L"Item1", player);
	widget->GetTransform()->SetRelativePosition(-0.65f, 0.6f, 1.f);
	widget->GetTransform()->SetRelativeScale(0.2f, 0.3f, 1.f);
	widget->SetOutHover([name]() {
		name->SetEnable(false);
		});
	widget->SetOnHover([widget, name]() {
		name->SetEnable(true);
		name->SetText(widget->GetItem()->GetItemInfo().name);
		std::cout << ws2s(widget->GetItem()->GetItemInfo().name) << '\n';
		});
	widget->SetOnClick([widget]() {
		CNetworkManager::GetInst()->s_GameSession->BuyItem(widget->GetItemId());
		});

	widget = CreateWidget<CItemButtonWidget>(L"Item2", player);
	widget->GetTransform()->SetRelativePosition(-0.25f, 0.6f, 1.f);
	widget->GetTransform()->SetRelativeScale(0.2f, 0.3f, 1.f);
	widget->SetOutHover([name]() {
		name->SetEnable(false);
		});
	widget->SetOnHover([widget, name]() {
		name->SetEnable(true);
		name->SetText(widget->GetItem()->GetItemInfo().name);
		std::cout << ws2s(widget->GetItem()->GetItemInfo().name) << '\n';
		});
	widget->SetOnClick([widget]() {
		CNetworkManager::GetInst()->s_GameSession->BuyItem(widget->GetItemId());
		});

	widget = CreateWidget<CItemButtonWidget>(L"Item3", player);
	widget->GetTransform()->SetRelativePosition(-0.25f, 0.1f, 1.f);
	widget->GetTransform()->SetRelativeScale(0.2f, 0.3f, 1.f);
	widget->SetOutHover([name]() {
		name->SetEnable(false);
		});
	widget->SetOnHover([widget, name]() {
		name->SetEnable(true);
		name->SetText(widget->GetItem()->GetItemInfo().name);
		std::cout << ws2s(widget->GetItem()->GetItemInfo().name) << '\n';
		});
	widget->SetOnClick([widget]() {
		CNetworkManager::GetInst()->s_GameSession->BuyItem(widget->GetItemId());
		});

	widget = CreateWidget<CItemButtonWidget>(L"Item4", player);
	widget->GetTransform()->SetRelativePosition(-0.65f, 0.1f, 1.f);
	widget->GetTransform()->SetRelativeScale(0.2f, 0.3f, 1.f);
	widget->SetOutHover([name]() {
		name->SetEnable(false);
		});
	widget->SetOnHover([widget, name]() {
		name->SetEnable(true);
		name->SetText(widget->GetItem()->GetItemInfo().name);
		std::cout << ws2s(widget->GetItem()->GetItemInfo().name) << '\n';
		});
	widget->SetOnClick([widget]() {
		CNetworkManager::GetInst()->s_GameSession->BuyItem(widget->GetItemId());
		});

	return true;
}

void CStoreWidgetWindow::Update()
{
	CWidgetWindow::Update();
}

void CStoreWidgetWindow::Render()
{
	ECamera_Type type = CRenderManager::GetInst()->GetMainCamera()->GetCameraType();

	if (ECamera_Type::Interaction_End == type)
	{
		SetEnable(false);
		return;
	}
	else if (ECamera_Type::Interaction != type)
		return;

	
	CWidgetWindow::Render();
}
