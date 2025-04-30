#pragma once
#include "WidgetWindow.h"
class CTextWindow :
	public CWidgetWindow
{
public:
	CTextWindow();
	virtual ~CTextWindow();

public:
	virtual bool Init(class CPlayer* player);

protected:
	std::unordered_map<std::wstring, ID2D1SolidColorBrush*> m_BrushMap;
	std::unordered_map<std::wstring, IDWriteTextFormat*> m_FontMap;

};

