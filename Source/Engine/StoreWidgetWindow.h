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
	void SetItemButtonEvent(class CItemButtonWidget* widget, struct ItemTooltip* tooltip, class CImageWidget* pannel);
	void UpdateSkill(int slotIndex, ESkillType type);

private:
	std::shared_ptr<struct ItemTooltip> m_Tooltip;
};

