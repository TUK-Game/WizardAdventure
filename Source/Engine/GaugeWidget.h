#pragma once
#include "ImageWidget.h"

class CGaugeWidget :
	public CImageWidget
{
public:
	void SetGaugeScale(float scale) { m_GaugeScale = scale; }
	void SetStartX(float x) { m_StartX = x; }

public:
	virtual bool Init() override;
	virtual void Update() override;

public:
	virtual CGaugeWidget* Clone() override { return new CGaugeWidget(*this); }

private:
	CGameObject* m_OwnerPlayer;
	// temp
	float		 m_MaxGauge = 100;
	float		 m_Gauge = 100;
	float		 m_GaugeScale;
	float		 m_StartX;
};

