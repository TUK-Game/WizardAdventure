#pragma once
#include <Engine/WidgetWindow.h>

class CSelectLevelWidgetWindow :
	public CWidgetWindow
{
public:
	CSelectLevelWidgetWindow();
	virtual ~CSelectLevelWidgetWindow();

public:
	virtual bool Init(class CPlayer* player);
};