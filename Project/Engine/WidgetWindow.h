#pragma once
#include "Ref.h"
#include "Widget.h"

class CWidgetWindow :
	public CRef
{
	friend class CLevel;

public:
    CWidgetWindow();
    virtual ~CWidgetWindow();

public:
    int GetZOrder() const { return m_zOrder; }
    class CWidget* GetWidget(int idx) { return m_vecWidget[idx]; }
    int GetWidgetCount() const { return (int)m_vecWidget.size(); }
    EWIDGETWINDOW_TYPE GetWindowType() { return m_Type; }
    class CPlayer* GetOwnerPlayer() { return m_OwnerPlayer; }

    void SetWindowType(EWIDGETWINDOW_TYPE type) { m_Type = type; }
    void SetZOrder(int z) { m_zOrder = z; }
    void SetOwnerPlayer(class CPlayer* player) { m_OwnerPlayer = player; }

    bool CheckWidget(class CWidget* widget)
    {
        for (auto& iter : m_vecWidget)
        {
            if (iter == widget)
                return true;
        }

        return false;
    }

public:
    virtual bool Init(class CPlayer* player);
    virtual void Update();
    virtual void FinalUpdate();
    virtual void Render();

public:
    CWidget* FindWidget(const std::wstring& name)
    {
        for (auto& iter : m_vecWidget)
        {
            if (iter->GetName() == name)
                return iter;
        }

        return nullptr;
    }

    template <typename T>
    T* CreateWidget(const std::wstring& name, class CPlayer* player)
    {
        T* widget = new T;
        widget->SetName(name);
        widget->m_owner = this;

        if (!widget->Init(player))
        {
            delete (widget);
            return nullptr;
        }

        m_vecWidget.push_back(widget);

        return widget;
    }

public:
    virtual CWidgetWindow* Clone() override { return new CWidgetWindow(*this); }

protected:
    int                               m_zOrder;
    std::vector<CSharedPtr<CWidget>>  m_vecWidget;
    CPlayer* m_OwnerPlayer;
    EWIDGETWINDOW_TYPE                m_Type;
};

