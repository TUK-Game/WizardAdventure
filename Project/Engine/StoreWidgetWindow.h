#pragma once
#include "WidgetWindow.h"

class CTextWidget;
struct ItemTooltip
{
	void Hide();
	void Show(const struct ItemInfo& info);
	void SetToolTip(CTextWidget* name, CTextWidget* description, CTextWidget* gold) { m_Name = name, m_Description = description; m_Price = gold; }

	CTextWidget* m_Name{};
	CTextWidget* m_Description{};
	CTextWidget* m_Price{};
};


class CStoreWidgetWindow :
	public CWidgetWindow
{
public:
	CStoreWidgetWindow();
	virtual ~CStoreWidgetWindow();

public:
	virtual bool Init(class CPlayer* player);
	virtual void Update();
	virtual void Render();

public:
	void SetEvent(class CItemButtonWidget* widget, ItemTooltip* tooltip);

private:
	ItemTooltip m_Tooltip;
};

