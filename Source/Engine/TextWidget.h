#pragma once
#include "Widget.h"
class CTextWidget :
	public CWidget
{
	friend class CWidgetWindow;
public:
	CTextWidget();
	virtual ~CTextWidget();

public:
	void SetSize(Vec2 size);
	void SetSize(float x, float y) { SetSize(Vec2(x, y)); }
	void SetPosition(Vec2 pos);
	void SetPosition(float x, float y) { SetPosition(Vec2(x, y)); }
	void SetText(const std::wstring& text) { m_Text = text; }
	void SetColor(ID2D1SolidColorBrush* color) { m_2dbrText = color; }
	void SetFont(IDWriteTextFormat* font) { m_Font = font; }

	void ArrangeRect();

public:
	virtual bool Init();
	virtual void Render();

public:
	virtual CTextWidget* Clone() override { return new CTextWidget(*this); }

protected:
	Vec2 m_ScreenSize;

	ID2D1DeviceContext2*	m_d2dDeviceContext{};
	IDWriteTextFormat*		m_Font{};
	ID2D1SolidColorBrush*	m_2dbrText{};
	Vec2					m_Size{};
	Vec2					m_Position{};
	D2D1_RECT_F				m_Rect{};

	std::wstring			m_Text{};
};

