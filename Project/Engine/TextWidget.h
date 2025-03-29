#pragma once
#include "Widget.h"
class CTextWidget :
	public CWidget
{
	friend class CWidgetWindow;
public:
	CTextWidget();
	virtual ~CTextWidget();

public:
	virtual bool Init();

public:
	virtual CTextWidget* Clone() override { return new CTextWidget(*this); }

protected:
	ID2D1DeviceContext2* m_d2dDeviceContext = NULL;

};

