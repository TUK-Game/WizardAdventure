#pragma once
#include "Widget.h" 

class CImageWidget :
	public CWidget
{
	friend class CWidgetWindow;

public:
    CImageWidget();
    virtual ~CImageWidget();

public:
    virtual void SetTexture(const std::wstring& name);

public:
    virtual CImageWidget* Clone() override { return new CImageWidget(*this); }
};

