#include "pch.h"
#include "PlayWidgetWindow.h"
#include "MeshRenderer.h"
#include "AssetManager.h"
#include "Transform.h"
#include "ImageWidget.h"
#include "SkillWidget.h"
#include "HpBar.h"
#include "TextRenderManager.h"
#include "Player.h"
#include "TextWidget.h"


CPlayWidgetWindow::CPlayWidgetWindow()
{
}

CPlayWidgetWindow::~CPlayWidgetWindow()
{
}

// HP : pos: -0.547, -0.855, 1
//      scale: 0.4, 0.025, 0.2

// SigantureGate : pos: -0.575, -0.93, 1
//				   scale: 0.34, 0.025, 0.2

// Skill1 : pos: -0.73, -0.722, 1
//			scale: 0.088, 0.135, 0.2

// Skill2 : pos: -0.605, -0.723, 1
//			scale: 0.08, 0.115, 0.2

// Skill3 : pos: -0.48, -0.725, 1
//			scale: 0.08, 0.115, 0.2

// Skill4 : pos: -0.372, -0.725, 1
//			scale: 0.08, 0.115, 0.2

// Skill5 : pos: -0.26, -0.725, 1
//			scale: 0.08, 0.115, 0.2

bool CPlayWidgetWindow::Init(CPlayer* player)
{
	CWidgetWindow::Init(player);
	
	{
		CImageWidget* SkillWidget = CreateWidget<CImageWidget>(L"SkillWidget", player);
		SkillWidget->SetTexture(L"SkillWidgetTexture");
		SkillWidget->GetTransform()->SetRelativePosition(-0.6f, -0.8f, 1.f);
		SkillWidget->GetTransform()->SetRelativeScale(0.8f, 0.4f, 0.2f);
	}

	{
		CHpBar* widget = CreateWidget<CHpBar>(L"HPBar", player);
		widget->SetTexture(L"Red");
		widget->GetTransform()->SetRelativePosition(-0.547f, -0.855f, 1.f);
		widget->GetTransform()->SetRelativeScale(0.4f, 0.025f, 0.2f);
		widget->SetGaugeScale(0.4f);
		widget->SetStartX(-0.547f);

	}

	{
		CGaugeWidget* widget = CreateWidget<CGaugeWidget>(L"SignautreGage", player);
		widget->SetTexture(L"Blue");
		widget->GetTransform()->SetRelativePosition(-0.575f, -0.93f, 1.f);
		widget->GetTransform()->SetRelativeScale(0.34f, 0.025f, 0.2f);
		widget->SetGaugeScale(0.34f);
		widget->SetStartX(-0.575f);

	}

	{
		CSkillWidget* SkillWidget = CreateWidget<CSkillWidget>(L"Skill1", player);
		SkillWidget->GetTransform()->SetRelativePosition(-0.73f, -0.722f, 1.f);
		SkillWidget->GetTransform()->SetRelativeScale(0.088f, 0.135f, 0.2f);
		SkillWidget->SetOriginCoolTime(100.f);
		SkillWidget->SetCoolTime(100.f);

		SkillWidget = CreateWidget<CSkillWidget>(L"Skill2", player);
		SkillWidget->GetTransform()->SetRelativePosition(-0.605f, -0.723f, 1.f);
		SkillWidget->GetTransform()->SetRelativeScale(0.08f, 0.115f, 0.2f);
		SkillWidget->SetOriginCoolTime(100.f);
		SkillWidget->SetCoolTime(100.f);

		SkillWidget = CreateWidget<CSkillWidget>(L"Skill3", player);
		SkillWidget->GetTransform()->SetRelativePosition(-0.48f, -0.725f, 1.f);
		SkillWidget->GetTransform()->SetRelativeScale(0.08f, 0.115f, 0.2f);
		SkillWidget->SetOriginCoolTime(100.f);
		SkillWidget->SetCoolTime(100.f);

		SkillWidget = CreateWidget<CSkillWidget>(L"Skill4", player);
		SkillWidget->GetTransform()->SetRelativePosition(-0.372f, -0.725f, 1.f);
		SkillWidget->GetTransform()->SetRelativeScale(0.08f, 0.115f, 0.2f);
		SkillWidget->SetOriginCoolTime(100.f);
		SkillWidget->SetCoolTime(100.f);

		SkillWidget = CreateWidget<CSkillWidget>(L"Skill5", player);
		SkillWidget->GetTransform()->SetRelativePosition(-0.26f, -0.725f, 1.f);
		SkillWidget->GetTransform()->SetRelativeScale(0.08f, 0.115f, 0.2f);
		SkillWidget->SetOriginCoolTime(100.f);
		SkillWidget->SetCoolTime(100.f);
	}

	// temp
	CTextWidget* w = CreateWidget<CTextWidget>(L"test123", player);
	w->GetTransform()->SetRelativePosition(0, -0.5, 0);
	w->GetTransform()->SetRelativeScale(1, 1, 1);
	w->SetBasicInfo(L"hiiii", L"궁서체", L"Black", Vec2(0, 0), Vec2(1000, 1000));

	return true;
}

void CPlayWidgetWindow::Update()
{
	CWidgetWindow::Update();
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
