#pragma once
#include "ImageWidget.h"

class CTexture;

class CButtonWidget : 
	public CImageWidget
{
	friend class CWidgetWindow;
public:
	CButtonWidget();
	virtual ~CButtonWidget();

public:
	virtual bool Init(class CPlayer* none) override;
	virtual void Update() override;

public:
	void SetFunction(std::function<void()> func);
	void SetButtonTexture(CTexture* defaultTex, CTexture* hoverTex, CTexture* pressedTex);
public:
	virtual CImageWidget* Clone() override { return new CImageWidget(*this); }
};

