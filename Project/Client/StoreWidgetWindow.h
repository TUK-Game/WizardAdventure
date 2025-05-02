#pragma once
#include <Engine/WidgetWindow.h>
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

};

