#pragma once
#include "Widget.h"

class CImageWidget :
	public CWidget
{
	friend class CWidgetWindow;

protected:
    CImageWidget();
    virtual ~CImageWidget();

public:
    virtual void SetTexture(const std::wstring& name);
    virtual CImageWidget* Clone() override { return new CImageWidget(*this); }
};

