#pragma once

#include <functional>

class CTexture;

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
    void SetOnHover(std::function<void()> func);  
    void SetOutHover(std::function<void()> func);
    void SetBTNTextures(CTexture* defaultTex, CTexture* hoverTex, CTexture* pressedTex);

    void UpdateTextureByState();

public:
    bool IsMouseOver();
    Vec2 GetMouseNDC();
private:
    std::function<void()> onClick;  
    std::function<void()> onHover;  
    std::function<void()> outHover;  
    bool m_bIsHovered = false;  
    bool m_bIsPressed = false;
    CTexture* m_DefaultTex = nullptr;
    CTexture* m_HoverTex = nullptr;
    CTexture* m_PressedTex = nullptr;

};