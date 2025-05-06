#pragma once
#include "WidgetWindow.h"
#include "SkillInfo.h"

class CInventoryWIdgetWindow :
	public CWidgetWindow
{
public:
	virtual bool Init(class CPlayer* player);

	void UpdateInventory();
	void SetEvent(class CItemButtonWidget* widget, struct ItemTooltip* tooltip);
	void SetSkill(int slotIndex, ESkillType type);
private:
	std::shared_ptr<struct ItemTooltip> m_Tooltip;
};

