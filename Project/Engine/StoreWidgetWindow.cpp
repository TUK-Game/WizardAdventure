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

	m_Tooltip = std::make_shared<ItemTooltip>();

	CImageWidget* backgroundWidget = CreateWidget<CImageWidget>(L"backgroundWidget", player);
	backgroundWidget->SetTexture(L"ShopBackGround");
	backgroundWidget->GetTransform()->SetRelativePosition(-0.4f, 0.f, 0.f);
	backgroundWidget->GetTransform()->SetRelativeScale(1.2f, 2.f, 0.2f);

	backgroundWidget = CreateWidget<CImageWidget>(L"Popup", player);
	backgroundWidget->SetTexture(L"Popup");
	backgroundWidget->GetTransform()->SetRelativePosition(0.55f, -0.5f, 0.f);
	backgroundWidget->GetTransform()->SetRelativeScale(0.8f, 0.8f, 0.2f);
	backgroundWidget->SetEnable(false);

	CTextWidget* name = CreateWidget<CTextWidget>(L"Text", player);
	name->GetTransform()->SetRelativePosition(-0.2f, -0.8f, 0.f);
	name->SetBasicInfo(L"", L"맑은 고딕_30", L"Yellow", Vec2(0.f, 0.f), Vec2(1000.f, 100.f));
	name->SetEnable(false);

	CTextWidget* description = CreateWidget<CTextWidget>(L"Description", player);
	description->GetTransform()->SetRelativePosition(-0.2f, -1.f, 0.f);
	description->SetBasicInfo(L"", L"맑은 고딕_20", L"White", Vec2(0.f, 0.f), Vec2(1000.f, 100.f));
	description->SetEnable(false);

	CTextWidget* price = CreateWidget<CTextWidget>(L"Gold", player);
	price->GetTransform()->SetRelativePosition(0.f, -0.8f, 0.f);
	price->SetBasicInfo(L"", L"맑은 고딕_30", L"White", Vec2(0.f, 0.f), Vec2(1000.f, 100.f));
	price->SetEnable(false);

	m_Tooltip->SetToolTip(name, description, price);

	CItemButtonWidget* widget = CreateWidget<CItemButtonWidget>(L"Item1", player);
	widget->GetTransform()->SetRelativePosition(-0.65f, 0.6f, 1.f);
	widget->GetTransform()->SetRelativeScale(0.2f, 0.3f, 1.f);
	SetEvent(widget, m_Tooltip.get(), backgroundWidget);

	widget = CreateWidget<CItemButtonWidget>(L"Item2", player);
	widget->GetTransform()->SetRelativePosition(-0.25f, 0.6f, 1.f);
	widget->GetTransform()->SetRelativeScale(0.2f, 0.3f, 1.f);
	SetEvent(widget, m_Tooltip.get(), backgroundWidget);

	widget = CreateWidget<CItemButtonWidget>(L"Item3", player);
	widget->GetTransform()->SetRelativePosition(-0.25f, 0.1f, 1.f);
	widget->GetTransform()->SetRelativeScale(0.2f, 0.3f, 1.f);
	SetEvent(widget, m_Tooltip.get(), backgroundWidget);

	widget = CreateWidget<CItemButtonWidget>(L"Item4", player);
	widget->GetTransform()->SetRelativePosition(-0.65f, 0.1f, 1.f);
	widget->GetTransform()->SetRelativeScale(0.2f, 0.3f, 1.f);
	SetEvent(widget, m_Tooltip.get(), backgroundWidget);


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

void CStoreWidgetWindow::SetEvent(CItemButtonWidget* widget, ItemTooltip* tooltip, CImageWidget* pannel)
{
	widget->SetOutHover([=]() {
		tooltip->Hide();
		pannel->SetEnable(false);
		});
	widget->SetOnHover([=]() {
		tooltip->Show(widget->GetItem()->GetItemInfo());
		pannel->SetEnable(true);
		});
	widget->SetOnClick([=]() {
		CNetworkManager::GetInst()->s_GameSession->BuyItem(widget->GetItemId());
		tooltip->Hide();
		pannel->SetEnable(false);
		});
}

