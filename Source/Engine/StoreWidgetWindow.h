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
	void SetItemButtonEvent(class CItemButtonWidget* widget, class CToolTip* tooltip, class CImageWidget* pannel);
	void SetNewSkillButtonEvent(class CSkillButtonWidget* widget, class CToolTip* tooltip, class CImageWidget* pannel);
	void SetSkillButtonEvent(class CButtonWidget* widget);
	void UpdateSkill(int slotIndex, ESkillType type);

private:
	std::shared_ptr<class CToolTip> m_Tooltip;
};

