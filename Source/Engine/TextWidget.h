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
	void SetBasicInfo(const std::wstring& text, const std::wstring& fontName, const std::wstring& colorName, const Vec2& pos, const Vec2& size);
	void SetText(const std::wstring& text) { m_Text = text;	 }

public:
	virtual bool Init(class CPlayer* player) override;
	virtual void Update() override;

	void UpdateTextPosition();
private:
	std::wstring m_Text{};
	std::wstring m_FontName{};
	std::wstring m_ColorName{};
	Vec2		 m_Pos{};
	Vec2		 m_Size{};
};

