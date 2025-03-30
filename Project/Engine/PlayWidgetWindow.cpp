#include "pch.h"
#include "PlayWidgetWindow.h"
#include "GaugeWidget.h"
#include "MeshRenderer.h"
#include "AssetManager.h"
#include "Transform.h"
#include "ImageWidget.h"
#include "SkillWidget.h"

CPlayWidgetWindow::CPlayWidgetWindow()
{
}

CPlayWidgetWindow::~CPlayWidgetWindow()
{
}

bool CPlayWidgetWindow::Init()
{
	CWidgetWindow::Init();
	
	{
		CGaugeWidget* widget = CreateWidget<CGaugeWidget>(L"HPBar");
		widget->SetTexture(L"Kita");
		widget->GetTransform()->SetRelativePosition(-0.65f, -0.8f, 1.f);
		widget->GetTransform()->SetRelativeScale(0.4f, 0.05f, 0.2f);
		widget->SetGaugeScale(0.4f);
		widget->SetStartX(-0.65f);

	}

	{
		CSkillWidget* SkillWidget = CreateWidget<CSkillWidget>(L"Skill");
		SkillWidget->SetTexture(L"Fireball");
		SkillWidget->GetTransform()->SetRelativePosition(-0.7f, -0.7f, 1.f);
		SkillWidget->GetTransform()->SetRelativeScale(0.1f, 0.1f, 0.2f);
		SkillWidget->SetOriginCoolTime(100.f);
		SkillWidget->SetCoolTime(100.f);
	}


	return true;
}
