#pragma once
#include "WidgetWindow.h"

class CInventoryWIdgetWindow :
	public CWidgetWindow
{
public:
	virtual bool Init(class CPlayer* player);

	void UpdateInventory();
	void SetEvent(class CItemButtonWidget* widget, struct ItemTooltip* tooltip);
private:
	std::shared_ptr<struct ItemTooltip> m_Tooltip;
};

