#include "pch.h"
#include "TextRenderManager.h"
#include "Device.h"
#include "Engine.h"

CTextRenderManager::CTextRenderManager()
{
}

CTextRenderManager::~CTextRenderManager()
{
}

bool CTextRenderManager::Init()
{   
	m_BrushMap = CDevice::GetInst()->GetBrushMap();
	m_FontMap = CDevice::GetInst()->GetFontMap();
		
    m_d2dDeviceContext = CDevice::GetInst()->m_d2dDeviceContext.Get();
    m_d2dDeviceContext->SetTransform(D2D1::Matrix3x2F::Identity());

    m_ScreenSize.x = CEngine::GetInst()->GetWindowInfo().Width;
    m_ScreenSize.y = CEngine::GetInst()->GetWindowInfo().Height;
	return true;
}

void CTextRenderManager::RenderText()
{
    for (const auto& req : m_Requests)
    {
        m_d2dDeviceContext->DrawTextW(req.text.c_str(), (UINT32)req.text.size(), req.font, req.rect, req.brush);
    }
    m_Requests.clear();
}

void CTextRenderManager::RequestTextRender(const std::wstring& text, const std::wstring& fontName, const std::wstring& colorName, const Vec2& pos, const Vec2& size)
{
    IDWriteTextFormat* font = m_FontMap[fontName];
    ID2D1SolidColorBrush* brush = m_BrushMap[colorName];
    if (!font || !brush)
        return;

    m_Requests.emplace_back(TextRenderRequest(text, pos, size, font, brush, m_ScreenSize));
}
