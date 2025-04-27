#pragma once
#include "ImageWidget.h"

class CGaugeWidget :
	public CImageWidget
{
public:
	void SetGaugeScale(float scale) { m_GaugeScale = scale; }
	void SetStartX(float x) { m_StartX = x; }
	void SetMaxGauge(float x) { m_MaxGauge = x; }
	void SetGauge(float x) { m_Gauge = x; }

public:
	virtual bool Init(class CPlayer* player) override;
	virtual void Update() override;

public:
	virtual CGaugeWidget* Clone() override { return new CGaugeWidget(*this); }

private:
	// temp
	float		 m_MaxGauge = 100;
	float		 m_Gauge = 100;
	float		 m_GaugeScale;
	float		 m_StartX;
};

