#pragma once
#include "WidgetWindow.h"
#include "SkillInfo.h"

class CInventoryWIdgetWindow :
	public CWidgetWindow
{
public:
	virtual bool Init(class CPlayer* player);

	void UpdateInventory();
	void SetEvent(class CItemButtonWidget* widget, class CToolTip* tooltip, class CImageWidget* pannel);
	void SetSkill(int slotIndex, ESkillType type);
private:
	std::shared_ptr<class CToolTip> m_Tooltip;
};

