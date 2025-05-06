#include "pch.h"
#include "ItemButtonWidget.h"
#include "MeshRenderer.h"
#include "Transform.h"
#include "AssetManager.h"
#include "UIButton.h"
#include "TextWidget.h"
#include "Item.h"

/////////////////////////ITEMTOOLTIP//////////////////////////////////

void ItemTooltip::Hide()
{
	if (m_Name) m_Name->SetEnable(false);
	if (m_Description) m_Description->SetEnable(false);
	if (m_Price) m_Price->SetEnable(false);
}

void ItemTooltip::Show(const ItemInfo& info)
{
	if (m_Name) { m_Name->SetEnable(true); m_Name->SetText(info.name); }
	if (m_Description) { m_Description->SetEnable(true); m_Description->SetText(info.description); }
	if (m_Price) { m_Price->SetEnable(true); m_Price->SetText(L"(" + std::to_wstring(info.price) + L"G)"); }
}


/////////////////////////ITEMBUTTONWIDGET//////////////////////////////////
CItemButtonWidget::CItemButtonWidget()
{
}

CItemButtonWidget::~CItemButtonWidget()
{
}

bool CItemButtonWidget::Init(CPlayer* none)
{
	CButtonWidget::Init(none);
	return true;
}

void CItemButtonWidget::Update()
{
	CButtonWidget::Update();
}



