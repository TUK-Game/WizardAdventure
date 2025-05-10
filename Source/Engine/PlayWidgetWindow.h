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
	virtual void Update() override;

	void SetGold();
	void SetSkill(ESkillType type, float coolTime, ESkillSlot slot);
	void SetGauge(const std::wstring& name, int gauge, bool bMax = false);
	void SetMaxHp(int maxHp);
};

