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
