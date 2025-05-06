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
#include "ButtonWidget.h"
#include "SkillButtonWidget.h"
#include "SkillData.h"
#include "ToolTip.h"

CStoreWidgetWindow::CStoreWidgetWindow()
{
}

CStoreWidgetWindow::~CStoreWidgetWindow()
{
}

bool CStoreWidgetWindow::Init(CPlayer* player)
{
	CWidgetWindow::Init(player);

	m_Tooltip = std::make_shared<CToolTip>();

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
	widget->GetTransform()->SetRelativePosition(-0.827f, 0.6f, 1.f);
	widget->GetTransform()->SetRelativeScale(0.145f, 0.15f, 1.f);
	SetItemButtonEvent(widget, m_Tooltip.get(), backgroundWidget);

	widget = CreateWidget<CItemButtonWidget>(L"Item2", player);
	widget->GetTransform()->SetRelativePosition(-0.555f, 0.6f, 1.f);
	widget->GetTransform()->SetRelativeScale(0.145f, 0.15f, 1.f);
	SetItemButtonEvent(widget, m_Tooltip.get(), backgroundWidget);

	widget = CreateWidget<CItemButtonWidget>(L"Item3", player);
	widget->GetTransform()->SetRelativePosition(-0.283f, 0.6f, 1.f);
	widget->GetTransform()->SetRelativeScale(0.145f, 0.15f, 1.f);
	SetItemButtonEvent(widget, m_Tooltip.get(), backgroundWidget);

	widget = CreateWidget<CItemButtonWidget>(L"Item4", player);
	widget->GetTransform()->SetRelativePosition(-0.011f, 0.6f, 1.f);
	widget->GetTransform()->SetRelativeScale(0.145f, 0.15f, 1.f);
	SetItemButtonEvent(widget, m_Tooltip.get(), backgroundWidget);

	widget = CreateWidget<CItemButtonWidget>(L"Item5", player);
	widget->GetTransform()->SetRelativePosition(-0.827f, 0.3f, 1.f);
	widget->GetTransform()->SetRelativeScale(0.145f, 0.15f, 1.f);
	SetItemButtonEvent(widget, m_Tooltip.get(), backgroundWidget);

	widget = CreateWidget<CItemButtonWidget>(L"Item6", player);
	widget->GetTransform()->SetRelativePosition(-0.555f, 0.3f, 1.f);
	widget->GetTransform()->SetRelativeScale(0.145f, 0.15f, 1.f);
	SetItemButtonEvent(widget, m_Tooltip.get(), backgroundWidget);

	widget = CreateWidget<CItemButtonWidget>(L"Item7", player);
	widget->GetTransform()->SetRelativePosition(-0.283f, 0.3f, 1.f);
	widget->GetTransform()->SetRelativeScale(0.145f, 0.15f, 1.f);
	SetItemButtonEvent(widget, m_Tooltip.get(), backgroundWidget);

	widget = CreateWidget<CItemButtonWidget>(L"Item8", player);
	widget->GetTransform()->SetRelativePosition(-0.011f, 0.3f, 1.f);
	widget->GetTransform()->SetRelativeScale(0.145f, 0.15f, 1.f);
	SetItemButtonEvent(widget, m_Tooltip.get(), backgroundWidget);

	CSkillButtonWidget* newSkillWidget = CreateWidget<CSkillButtonWidget>(L"NewSkill1", player);
	newSkillWidget->GetTransform()->SetRelativePosition(-0.693f, -0.153f, 1.f);
	newSkillWidget->GetTransform()->SetRelativeScale(0.145f, 0.15f, 1.f);
	SetNewSkillButtonEvent(newSkillWidget, m_Tooltip.get(), backgroundWidget);

	newSkillWidget = CreateWidget<CSkillButtonWidget>(L"NewSkill2", player);
	newSkillWidget->GetTransform()->SetRelativePosition(-0.422, -0.153f, 1.f);
	newSkillWidget->GetTransform()->SetRelativeScale(0.145f, 0.15f, 1.f);
	SetNewSkillButtonEvent(newSkillWidget, m_Tooltip.get(), backgroundWidget);

	newSkillWidget = CreateWidget<CSkillButtonWidget>(L"NewSkill3", player);
	newSkillWidget->GetTransform()->SetRelativePosition(-0.151f, -0.153f, 1.f);
	newSkillWidget->GetTransform()->SetRelativeScale(0.145f, 0.15f, 1.f);
	SetNewSkillButtonEvent(newSkillWidget, m_Tooltip.get(), backgroundWidget);

	CButtonWidget* skillWidget = CreateWidget<CButtonWidget>(L"SSkill1", player);
	skillWidget->GetTransform()->SetRelativePosition(-0.601f, -0.582f, 1.f);
	skillWidget->GetTransform()->SetRelativeScale(0.135f, 0.15f, 1.f);
	skillWidget->SetEnable(false);

	skillWidget = CreateWidget<CButtonWidget>(L"SSkill2", player);
	skillWidget->GetTransform()->SetRelativePosition(-0.272f, -0.585f, 1.f);
	skillWidget->GetTransform()->SetRelativeScale(0.15f, 0.16f, 1.f);
	skillWidget->SetEnable(false);

	skillWidget = CreateWidget<CButtonWidget>(L"SSkill3", player);
	skillWidget->GetTransform()->SetRelativePosition(-0.76f, -0.782f, 1.f);
	skillWidget->GetTransform()->SetRelativeScale(0.13f, 0.125f, 1.f);
	skillWidget->SetEnable(false);

	skillWidget = CreateWidget<CButtonWidget>(L"SSkill4", player);
	skillWidget->GetTransform()->SetRelativePosition(-0.425f, -0.782f, 1.f);
	skillWidget->GetTransform()->SetRelativeScale(0.13f, 0.125f, 1.f);
	skillWidget->SetEnable(false);

	skillWidget = CreateWidget<CButtonWidget>(L"SSkill5", player);
	skillWidget->GetTransform()->SetRelativePosition(-0.09f, -0.782f, 1.f);
	skillWidget->GetTransform()->SetRelativeScale(0.13f, 0.125f, 1.f);
	skillWidget->SetEnable(false);

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

void CStoreWidgetWindow::SetItemButtonEvent(CItemButtonWidget* widget, CToolTip* tooltip, CImageWidget* pannel)
{
	widget->SetOutHover([=]() {
		tooltip->Hide();
		pannel->SetEnable(false);
		});
	widget->SetOnHover([=]() {
		tooltip->Show<ItemInfo>(widget->GetItem()->GetItemInfo());
		pannel->SetEnable(true);
		});	
	widget->SetOnClick([=]() {
		CNetworkManager::GetInst()->s_GameSession->BuyItem(widget->GetItemId());
		tooltip->Hide();
		pannel->SetEnable(false);
		});
}

void CStoreWidgetWindow::SetNewSkillButtonEvent(CSkillButtonWidget* widget, CToolTip* tooltip, CImageWidget* pannel)
{
	widget->SetOutHover([=]() {
		tooltip->Hide();
		pannel->SetEnable(false);
		});
	widget->SetOnHover([=]() {
		tooltip->Show<SkillInfo>(widget->GetSkill()->GetSkillInfo());
		pannel->SetEnable(true);
		});
	widget->SetOnClick([=]() {
		CNetworkManager::GetInst()->s_GameSession->BuySkill(widget->GetSkillId());
		tooltip->Hide();
		pannel->SetEnable(false);
		});
}

void CStoreWidgetWindow::SetSkillButtonEvent(CButtonWidget* widget)
{
	widget->SetOutHover([=]() {
		});
	widget->SetOnHover([=]() {
		});
	widget->SetOnClick([=]() {
		});
}

void CStoreWidgetWindow::UpdateSkill(int slotIndex, ESkillType type)
{
	CButtonWidget* widget = dynamic_cast<CButtonWidget*>(FindWidget(L"SSkill" + std::to_wstring(slotIndex + 1)));
	const std::wstring& skillName = CAssetManager::GetInst()->ConvertSkillToTextureName(type);
	widget->SetTexture(skillName);
	widget->SetEnable(true);
}
