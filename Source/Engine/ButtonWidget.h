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
	void SetOutHover(std::function<void()> func);
	void SetOnHover(std::function<void()> func);
	void SetOnClick(std::function<void()> func);
	void SetButtonTexture(CTexture* defaultTex, CTexture* hoverTex, CTexture* pressedTex);
public:
	virtual CButtonWidget* Clone() override { return new CButtonWidget(*this); }
};

