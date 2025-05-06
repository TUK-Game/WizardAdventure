#pragma once
#include "WidgetWindow.h"

class CTextWidget;


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
	void SetEvent(class CItemButtonWidget* widget, struct ItemTooltip* tooltip);

private:
	std::shared_ptr<struct ItemTooltip> m_Tooltip;
};

