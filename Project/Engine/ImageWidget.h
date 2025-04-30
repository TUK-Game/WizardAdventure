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
    
    virtual bool Init(class CPlayer* player);

public:
    virtual CImageWidget* Clone() override { return new CImageWidget(*this); }
};

