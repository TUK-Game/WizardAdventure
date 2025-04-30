#pragma once
#include "WidgetWindow.h"
class TestWidget :
	public CWidgetWindow
{
public:
    TestWidget();
    virtual ~TestWidget();

public:
    virtual bool Init(class CPlayer* player);
    void AddPlayer(class CPlayer* player, int idx);
};

