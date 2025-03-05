#pragma once

#include <functional>

class CUIButton : public CComponent
{
public:
    CUIButton();
    virtual ~CUIButton();

public:
    virtual void Update() override;
    virtual void FinalUpdate() override;
    virtual CUIButton* Clone() override { return new CUIButton(*this); }

    void SetOnClick(std::function<void()> func);  
public:
    bool IsMouseOver();
    Vec2 GetMouseNDC();
private:
    std::function<void()> onClick;  
    bool m_bIsHovered = false;  

};