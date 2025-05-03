#include "pch.h"
#include "TextWidget.h"
#include "TextRenderManager.h"
#include "Transform.h"
CTextWidget::CTextWidget()
{
}

CTextWidget::~CTextWidget()
{
}

void CTextWidget::SetBasicInfo(const std::wstring& text, const std::wstring& fontName, const std::wstring& colorName, const Vec2& pos, const Vec2& size)
{
	m_Text = text; m_FontName = fontName; m_ColorName = colorName; m_Pos = pos; m_Size = size;
}

bool CTextWidget::Init(CPlayer* player)
{
	CWidget::Init(player);
	AddComponent(new CTransform);
	return true;
}

void CTextWidget::Update()
{
	CWidget::Update();
	UpdateTextPosition();
	CTextRenderManager::GetInst()->RequestTextRender(m_Text, m_FontName, m_ColorName, m_Pos, m_Size);
}

void CTextWidget::UpdateTextPosition()
{		
	Vec3 parentPos = GetTransform()->GetRelativePosition();
	Vec3 parentSize = GetTransform()->GetRelativeScale();

	m_Pos = Vec2(parentPos.x, parentPos.y);
	m_Pos.x += parentSize.x / 2.f;
	m_Pos.y += parentSize.y / 2.f;
}
