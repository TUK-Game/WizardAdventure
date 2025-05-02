#include "pch.h"
#include "ItemButtonWidget.h"
#include "MeshRenderer.h"
#include "Transform.h"
#include "AssetManager.h"
#include "UIButton.h"

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
