#include "pch.h"
#include "PlayWidgetWindow.h"
#include "MeshRenderer.h"
#include "AssetManager.h"
#include "Transform.h"
#include "ImageWidget.h"
#include "SkillWidget.h"
#include "HpBar.h"

CPlayWidgetWindow::CPlayWidgetWindow()
{
}

CPlayWidgetWindow::~CPlayWidgetWindow()
{
}

// HP : pos: -0.748, -0.925, 1
//      scale: 0.287, 0.04, 0.2

// SigantureGate : pos: -0.767, -0.972, 1
//				   scale: 0.248, 0.035, 0.2

// Skill4 : pos: -0.665, -0.815, 1
//			scale: 0.053, 0.086, 0.2

bool CPlayWidgetWindow::Init()
{
	CWidgetWindow::Init();
	
	{
		CImageWidget* SkillWidget = CreateWidget<CImageWidget>(L"SkillWidget4");
		SkillWidget->SetTexture(L"SkillWidgetTexture");
		SkillWidget->GetTransform()->SetRelativePosition(-0.75f, -0.875f, 1.f);
		SkillWidget->GetTransform()->SetRelativeScale(0.5f, 0.25f, 0.2f);
	}

	{
		CHpBar* widget = CreateWidget<CHpBar>(L"HPBar");
		widget->SetTexture(L"Kita");
		widget->GetTransform()->SetRelativePosition(-0.748f, -0.925f, 1.f);
		widget->GetTransform()->SetRelativeScale(0.287f, 0.04f, 0.2f);
		widget->SetGaugeScale(0.287f);
		widget->SetStartX(-0.748f);

	}

	{
		CGaugeWidget* widget = CreateWidget<CGaugeWidget>(L"SignautreGage");
		widget->SetTexture(L"Ryo");
		widget->GetTransform()->SetRelativePosition(-0.767f, -0.972f, 1.f);
		widget->GetTransform()->SetRelativeScale(0.248f, 0.035f, 0.2f);
		widget->SetGaugeScale(0.248f);
		widget->SetStartX(-0.767f);

	}

	{
		CSkillWidget* SkillWidget = CreateWidget<CSkillWidget>(L"Skill4");
		//SkillWidget->SetTexture(L"Fireball");
		SkillWidget->GetTransform()->SetRelativePosition(-0.665f, -0.815f, 1.f);
		SkillWidget->GetTransform()->SetRelativeScale(0.053f, 0.086f, 0.2f);
		SkillWidget->SetOriginCoolTime(100.f);
		SkillWidget->SetCoolTime(100.f);
	}
	return true;
}

void CPlayWidgetWindow::SetSkill(int idx, const std::wstring& textureName, float coolTime)
{
	std::wstring name = L"Skill" + std::to_wstring(idx);
	CSkillWidget* widget = dynamic_cast<CSkillWidget*>(FindWidget(name));
	if (widget)
	{
		widget->SetTexture(textureName);
		widget->SetOriginCoolTime(coolTime);
		widget->SetCoolTime(0.f);
	}
}

void CPlayWidgetWindow::SetGauge(const std::wstring& name, int gauge, bool bMax)
{
	CGaugeWidget* widget = dynamic_cast<CGaugeWidget*>(FindWidget(name));
	if (bMax)
	{
		widget->SetMaxGauge(gauge);
	}
	widget->SetGauge(gauge);
}

void CPlayWidgetWindow::SetMaxHp(int maxHp)
{
}
