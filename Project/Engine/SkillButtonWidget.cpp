#include "pch.h"
#include "SkillButtonWidget.h"

CSkillButtonWidget::CSkillButtonWidget()
{
}

CSkillButtonWidget::~CSkillButtonWidget()
{
}

bool CSkillButtonWidget::Init(CPlayer* none)
{
	CButtonWidget::Init(none);
	return true;
}

void CSkillButtonWidget::Update()
{
	CButtonWidget::Update();
}
