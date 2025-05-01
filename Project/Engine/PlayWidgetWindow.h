#pragma once
#include "WidgetWindow.h"
#include "SkillInfo.h"

class CPlayWidgetWindow :
	public CWidgetWindow
{
public:
	CPlayWidgetWindow();
	virtual ~CPlayWidgetWindow();
public:
	virtual bool Init(class CPlayer* player);

	void SetSkill(int idx, const std::wstring& skillName, float coolTime, ESkillSlot slot);
	void SetGauge(const std::wstring& name, int gauge, bool bMax = false);
	void SetMaxHp(int maxHp);
};

