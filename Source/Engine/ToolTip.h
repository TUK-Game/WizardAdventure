#pragma once
#include "TextWidget.h"

class CToolTip
{
public:

	void Hide();
	void SetToolTip(CTextWidget* name, CTextWidget* description, CTextWidget* gold) { m_Name = name, m_Description = description; m_Price = gold; }

	template<class T>
	void Show(const T& info)
	{
		if (m_Name) { m_Name->SetEnable(true); m_Name->SetText(info.name); }
		if (m_Description) { m_Description->SetEnable(true); m_Description->SetText(info.description); }
		if (m_Price) { m_Price->SetEnable(true); m_Price->SetText(L"(" + std::to_wstring(info.price) + L"G)"); }
	}

	CTextWidget* m_Name{};
	CTextWidget* m_Description{};
	CTextWidget* m_Price{};
};
