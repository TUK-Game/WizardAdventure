#pragma once
#include "WidgetWindow.h"
class CPlayWidgetWindow :
	public CWidgetWindow
{
public:
	CPlayWidgetWindow();
	virtual ~CPlayWidgetWindow();
public:
	virtual bool Init(class CPlayer* player);
	virtual void Update() override;

public:
	void SetSkill(int idx, const std::wstring& skillName, float coolTime);
	void SetGauge(const std::wstring& name, int gauge, bool bMax = false);
	void SetMaxHp(int maxHp);
};

