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
    std::vector<std::shared_ptr<class CItem>>& GetItemList() { return m_ItemList; }
    void PushItem(std::shared_ptr<class CItem> item) { m_ItemList.push_back(item); }

public:
    void CreateStateManager();
    Vec3 InteractionCameraPos(Vec3& rot);
public:
    virtual void Interation();
    virtual void ShowWidgetWindow();

protected:
    std::wstring m_WidgetWindowName;
    EWIDGETWINDOW_TYPE m_WindowType;

private:
    std::vector<std::shared_ptr<class CItem>> m_ItemList;
};

