#include "pch.h"
#include "ItemButtonWidget.h"
#include "MeshRenderer.h"
#include "Transform.h"
#include "AssetManager.h"
#include "UIButton.h"
#include "TextWidget.h"
#include "Item.h"

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



