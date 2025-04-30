#pragma once
#include "GaugeWidget.h"

class CHpBar :
	public CGaugeWidget
{
public:
	virtual CGaugeWidget* Clone() override { return new CGaugeWidget(*this); }

	virtual void Update() override;
};

