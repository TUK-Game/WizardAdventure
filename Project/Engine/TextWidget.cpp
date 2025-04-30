#include "pch.h"
#include "TextWidget.h"
#include "AssetManager.h"
#include "Device.h"
#include "Engine.h"

CTextWidget::CTextWidget()
{
}

CTextWidget::~CTextWidget()
{
}

void CTextWidget::SetSize(Vec2 size)
{
	m_Size = size;
	ArrangeRect();
}

void CTextWidget::SetPosition(Vec2 pos)
{
	m_Position.x = (pos.x + 1.f) * 0.5f * m_ScreenSize.x;
	m_Position.y = (1.f - pos.y) * 0.5f * m_ScreenSize.y;

	ArrangeRect();
}

void CTextWidget::ArrangeRect()
{
	Vec2 pos = m_Position;
	m_Rect.left = m_Position.x - (m_Size.x / 2);
	m_Rect.right = m_Position.x + (m_Size.x / 2);
	m_Rect.bottom = m_Position.y - (m_Size.y / 2);
	m_Rect.top = m_Position.y + (m_Size.y / 2);
}

bool CTextWidget::Init(CPlayer* player)
{
	CWidget::Init(player);
	m_d2dDeviceContext = CDevice::GetInst()->m_d2dDeviceContext.Get();
	m_d2dDeviceContext->SetTransform(D2D1::Matrix3x2F::Identity());

	m_ScreenSize.x = CEngine::GetInst()->GetWindowInfo().Width;
	m_ScreenSize.y = CEngine::GetInst()->GetWindowInfo().Height;

	return true;
}

void CTextWidget::Render()
{
	m_d2dDeviceContext->DrawTextW(m_Text.c_str(), (UINT32)m_Text.size(), m_Font, &m_Rect, m_2dbrText);
}


