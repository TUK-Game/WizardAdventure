#pragma once
#include <Engine/WidgetWindow.h>

class CStartLevelWidgetWindow :
	public CWidgetWindow
{
public:
	CStartLevelWidgetWindow();
	virtual ~CStartLevelWidgetWindow();

public:
	virtual bool Init();
};

