#pragma once
#include <ImGui/imgui.h>

struct TextRenderRequest
{
	std::wstring text;
	Vec2 position;
	Vec2 fontSize;
	D2D1_RECT_F	rect;
	IDWriteTextFormat* font;
	ID2D1SolidColorBrush* brush{};

	TextRenderRequest(const std::wstring& text, const Vec2& pos, const Vec2& size, IDWriteTextFormat* font, ID2D1SolidColorBrush* brush, const Vec2& screenSize)
		: text(text), fontSize(size), font(font), brush(brush)
	{
		position.x = (pos.x + 1.f) * 0.5f * screenSize.x;
		position.y = (1.f - pos.y) * 0.5f * screenSize.y;

		rect.left = position.x - (size.x / 2.f);
		rect.right = position.x + (size.x / 2.f);
		rect.bottom = position.y - (size.y / 2.f);
		rect.top = position.y + (size.y / 2.f);
	}
};


class CTextRenderManager :
	public CSingleton<CTextRenderManager>
{
	DECLARE_SINGLETON(CTextRenderManager)

public:
	Vec2 GetScreenSize() { return m_ScreenSize; }

public:
	bool Init();
	void RenderText();
	void RequestTextRender(const std::wstring& text, const std::wstring& fontName, const std::wstring& colorName, const Vec2& pos, const Vec2& size);

private:
	Vec2 m_ScreenSize;

	ID2D1DeviceContext2* m_d2dDeviceContext{};

	std::unordered_map<std::wstring, ID2D1SolidColorBrush*> m_BrushMap;
	std::unordered_map<std::wstring, IDWriteTextFormat*> m_FontMap;

	std::vector<TextRenderRequest> m_Requests;

};


