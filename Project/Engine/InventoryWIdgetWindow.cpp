#include "pch.h"
#include "InventoryWIdgetWindow.h"
#include "ImageWidget.h"
#include "Transform.h"
#include "ItemButtonWidget.h"
#include "Item.h"
#include "Player.h"
#include "AssetManager.h"
#include "PathManager.h"

bool CInventoryWIdgetWindow::Init(CPlayer* player)
{
	CWidgetWindow::Init(player);

	m_Tooltip = std::make_shared<ItemTooltip>();

	CImageWidget* backgroundWidget = CreateWidget<CImageWidget>(L"Inventory", player);
	backgroundWidget->SetTexture(L"Inventory");
	backgroundWidget->GetTransform()->SetRelativePosition(-0.4f, 0.f, 0.f);
	backgroundWidget->GetTransform()->SetRelativeScale(1.8f, 2.1f, 0.2f);

	CImageWidget* skillWidget = CreateWidget<CImageWidget>(L"ISkill1", player);
	skillWidget->GetTransform()->SetRelativePosition(-0.878f, 0.075f, 1.f);
	skillWidget->GetTransform()->SetRelativeScale(0.142f, 0.172f, 1.f);
	skillWidget->SetEnable(false);

	skillWidget = CreateWidget<CImageWidget>(L"ISkill2", player);
	skillWidget->GetTransform()->SetRelativePosition(-0.647f, 0.076f, 1.f);
	skillWidget->GetTransform()->SetRelativeScale(0.15f, 0.175f, 1.f);
	skillWidget->SetEnable(false);

	skillWidget = CreateWidget<CImageWidget>(L"ISkill3", player);
	skillWidget->GetTransform()->SetRelativePosition(-0.417f, 0.072f, 1.f);
	skillWidget->GetTransform()->SetRelativeScale(0.135f, 0.143f, 1.f);
	skillWidget->SetEnable(false);

	skillWidget = CreateWidget<CImageWidget>(L"ISkill4", player);
	skillWidget->GetTransform()->SetRelativePosition(-0.213f, 0.072f, 1.f);
	skillWidget->GetTransform()->SetRelativeScale(0.135f, 0.143f, 1.f);
	skillWidget->SetEnable(false);

	skillWidget = CreateWidget<CImageWidget>(L"ISkill5", player);
	skillWidget->GetTransform()->SetRelativePosition(-0.01f, 0.072f, 1.f);
	skillWidget->GetTransform()->SetRelativeScale(0.135f, 0.143f, 1.f);
	skillWidget->SetEnable(false);


	CItemButtonWidget* widget = CreateWidget<CItemButtonWidget>(L"Inven1", player);
	widget->GetTransform()->SetRelativePosition(-0.823f, -0.235f, 1.f);
	widget->GetTransform()->SetRelativeScale(0.17f, 0.2f, 1.f);
	SetEvent(widget, m_Tooltip.get());

	widget = CreateWidget<CItemButtonWidget>(L"Inven2", player);
	widget->GetTransform()->SetRelativePosition(-0.553f, -0.235f, 1.f);
	widget->GetTransform()->SetRelativeScale(0.17f, 0.2f, 1.f);
	SetEvent(widget, m_Tooltip.get());

	widget = CreateWidget<CItemButtonWidget>(L"Inven3", player);
	widget->GetTransform()->SetRelativePosition(-0.283f, -0.235f, 1.f);
	widget->GetTransform()->SetRelativeScale(0.17f, 0.2f, 1.f);
	SetEvent(widget, m_Tooltip.get());

	widget = CreateWidget<CItemButtonWidget>(L"Inven4", player);
	widget->GetTransform()->SetRelativePosition(-0.007f, -0.235f, 1.f);
	widget->GetTransform()->SetRelativeScale(0.17f, 0.2f, 1.f);
	SetEvent(widget, m_Tooltip.get());

	widget = CreateWidget<CItemButtonWidget>(L"Inven5", player);
	widget->GetTransform()->SetRelativePosition(-0.823f, -0.57f, 1.f);
	widget->GetTransform()->SetRelativeScale(0.17f, 0.2f, 1.f);
	SetEvent(widget, m_Tooltip.get());

	widget = CreateWidget<CItemButtonWidget>(L"Inven6", player);
	widget->GetTransform()->SetRelativePosition(-0.553f, -0.57f, 1.f);
	widget->GetTransform()->SetRelativeScale(0.17f, 0.2f, 1.f);
	SetEvent(widget, m_Tooltip.get());

	widget = CreateWidget<CItemButtonWidget>(L"Inven7", player);
	widget->GetTransform()->SetRelativePosition(-0.283f, -0.57f, 1.f);
	widget->GetTransform()->SetRelativeScale(0.17f, 0.2f, 1.f);
	SetEvent(widget, m_Tooltip.get());

	widget = CreateWidget<CItemButtonWidget>(L"Inven8", player);
	widget->GetTransform()->SetRelativePosition(-0.007f, -0.57f, 1.f);
	widget->GetTransform()->SetRelativeScale(0.17f, 0.2f, 1.f);
	SetEvent(widget, m_Tooltip.get());

	return true;
}

void CInventoryWIdgetWindow::UpdateInventory()
{
	const auto& items = m_OwnerPlayer->GetItems();
	for (int i = 0; i < items.size(); ++i)
	{
		std::wstring name = L"Inven" + std::to_wstring(i + 1);
		CItemButtonWidget* widget = dynamic_cast<CItemButtonWidget*>(FindWidget(name));
		std::cout << ws2s(name) << "\n";
		if (widget)
		{
			std::wstring itemName = items[i]->GetItemInfo().name;
			CTexture* texture = CAssetManager::GetInst()->FindAsset<CTexture>(itemName);
			widget->SetItemId(items[i]->GetItemInfo().id);
			widget->SetItem(items[i]);
			widget->SetButtonTexture(	
				texture,
				texture,
				texture
			);
			widget->SetEnable(true);
		}
	}

}

void CInventoryWIdgetWindow::SetEvent(CItemButtonWidget* widget, ItemTooltip* tooltip)
{
	widget->SetOutHover([=]() {
		tooltip->Hide();
		});
	widget->SetOnHover([=]() {
		tooltip->Show(widget->GetItem()->GetItemInfo());
		});
	widget->SetOnClick([=]() {
		tooltip->Hide();
		});

	widget->SetEnable(false);
}

void CInventoryWIdgetWindow::SetSkill(int slotIndex, ESkillType type)
{
	CImageWidget* widget = dynamic_cast<CImageWidget*>(FindWidget(L"ISkill" + std::to_wstring(slotIndex + 1)));
	const std::wstring& skillName = CAssetManager::GetInst()->ConvertSkillToTextureName(type);
	widget->SetTexture(skillName);
	widget->SetEnable(true);
}
