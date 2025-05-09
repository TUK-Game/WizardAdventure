#pragma once
#include "GameObject.h"

class CNPC : public CGameObject
{
public:
    CNPC();
    virtual ~CNPC();

public:
    void SetWidgetWindowName(const std::wstring& name) { m_WidgetWindowName = name; }
    void SetWidgetWindowType(EWIDGETWINDOW_TYPE type) { m_WindowType = type; }
    virtual void Begin();
    virtual void Update();
    virtual void FinalUpdate();
    virtual void Render();

public:
    void CreateStateManager();

public:
    virtual void Interation();
    virtual void SuccessInteration();
    virtual void FailInteration();
    virtual void ShowWidgetWindow();

protected:
    std::wstring m_WidgetWindowName;
    EWIDGETWINDOW_TYPE m_WindowType;
};

