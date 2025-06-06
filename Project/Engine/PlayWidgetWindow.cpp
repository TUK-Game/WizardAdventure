﻿#include "pch.h"
#include "PlayWidgetWindow.h"
#include "MeshRenderer.h"
#include "AssetManager.h"
#include "Transform.h"
#include "ImageWidget.h"
#include "SkillWidget.h"
#include "HpBar.h"
#include "Player.h"
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
		CImageWidget* face = CreateWidget<CImageWidget>(L"Face", player);
		face->GetTransform()->SetRelativePosition(-0.89f, -0.805f, 1.f);
		face->GetTransform()->SetRelativeScale(0.163f, 0.27f, 0.2f);
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

	{
		Stats* stats = player->GetStats();
		CTextWidget* gold = CreateWidget<CTextWidget>(L"PGold", player);
		gold->GetTransform()->SetRelativePosition(-0.75f, -1.42f, 1.f);
		gold->SetBasicInfo(std::format(L"{}", stats->gold), L"맑은 고딕_20", L"Yellow", Vec2(0.f, 0.f), Vec2(500.f, 100.f));
	}

	return true;
}

void CPlayWidgetWindow::Update()
{
	CWidgetWindow::Update();
}

void CPlayWidgetWindow::SetGold()
{
	Stats* stats = m_OwnerPlayer->GetStats();
	std::wstring name = L"PGold";
	CTextWidget* widget = dynamic_cast<CTextWidget*>(FindWidget(name));
	widget->SetBasicInfo(std::format(L"{}", stats->gold), L"맑은 고딕_20", L"Yellow", Vec2(0.f, 0.f), Vec2(500.f, 100.f));
}

void CPlayWidgetWindow::SetSkill(ESkillType type, float coolTime, ESkillSlot slot)
{
	std::wstring name = L"Skill" + std::to_wstring(slot + 1);
	CSkillWidget* widget = dynamic_cast<CSkillWidget*>(FindWidget(name));
	const std::wstring& skillName = CAssetManager::GetInst()->ConvertSkillToTextureName(type);
	if (widget)
	{
		widget->SetTexture(skillName);
		widget->SetOriginCoolTime(coolTime);
		widget->SetCoolTime(0.f);
		widget->SetSlot(slot);
	}
}

void CPlayWidgetWindow::SetGauge(const std::wstring& name, int gauge, bool bMax)
{
	CGaugeWidget* widget = dynamic_cast<CGaugeWidget*>(FindWidget(name));
	if (bMax)
	{
		widget->SetMaxGauge(gauge);
	}
	else
	{
		widget->SetGauge(gauge);
	}
}

void CPlayWidgetWindow::SetMaxHp(int maxHp)
{
}

void CPlayWidgetWindow::SetFace(const std::wstring& name)
{
	std::wstring wname = L"Face";
	CImageWidget* widget = dynamic_cast<CImageWidget*>(FindWidget(wname));
	
	if (widget)
	{
		widget->SetTexture(name);
	}
}
