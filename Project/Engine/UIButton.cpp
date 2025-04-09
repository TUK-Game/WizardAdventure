#include "pch.h"
#include "Engine.h"
#include "Transform.h"
#include "UIButton.h"
#include "UI.h"
#include "GameObject.h"
#include "AssetManager.h"
#include "MeshRenderer.h"

CUIButton::CUIButton() : CComponent(EComponent_Type::UIButton)
{
}

CUIButton::~CUIButton()
{
}

void CUIButton::Update()
{
    bool isMouseOver = IsMouseOver();
    bool isPressed = (GetAsyncKeyState(VK_LBUTTON) & 0x8000);

    if (isMouseOver)
    {
        if (!m_bIsHovered)
        {
            m_bIsHovered = true;
            UpdateTextureByState(); // 상태 변경 시 텍스처 갱신
        }

        if (isPressed && !m_bIsPressed)
        {
            m_bIsPressed = true;
            UpdateTextureByState();
        }
        else if (!isPressed && m_bIsPressed)
        {
            m_bIsPressed = false;
            UpdateTextureByState();

            CAssetManager::GetInst()->SoundPlay("Click");
            if (onClick)
                onClick();
        }
    }
    else
    {
        if (m_bIsHovered || m_bIsPressed)
        {
            m_bIsHovered = false;
            m_bIsPressed = false;
            UpdateTextureByState();
        }
    }

}

void CUIButton::FinalUpdate()
{
}

void CUIButton::SetOnClick(std::function<void()> func)
{
    onClick = func;
}

void CUIButton::SetBTNTextures(CTexture* defaultTex, CTexture* hoverTex, CTexture* pressedTex)
{
    m_DefaultTex = defaultTex;
    m_HoverTex = hoverTex;
    m_PressedTex = pressedTex;

    // 초기 텍스처 설정
    GetOwner()->GetMeshRenderer()->GetMaterial()->SetTexture(0, m_DefaultTex);
}

Vec2 CUIButton::GetMouseNDC()
{
    POINT mousePos;
    GetCursorPos(&mousePos);
    ScreenToClient(GetActiveWindow(), &mousePos);

    RECT rect;
    GetClientRect(GetActiveWindow(), &rect);

    float ndcX = (mousePos.x / (float)rect.right) * 2.0f - 1.0f;
    float ndcY = 1.0f - (mousePos.y / (float)rect.bottom) * 2.0f;

    return Vec2(ndcX, ndcY);
}

bool CUIButton::IsMouseOver()
{
    Vec3 uiPos = GetOwner()->GetTransform()->GetRelativePosition();
    Vec3 uiScale = GetOwner()->GetTransform()->GetRelativeScale();
    Vec2 mousePos = GetMouseNDC();

    float left = uiPos.x - uiScale.x * 0.5f;
    float right = uiPos.x + uiScale.x * 0.5f;
    float top = uiPos.y + uiScale.y * 0.5f;
    float bottom = uiPos.y - uiScale.y * 0.5f;

    return (mousePos.x >= left && mousePos.x <= right &&
        mousePos.y >= bottom && mousePos.y <= top);
}

void CUIButton::UpdateTextureByState()
{
    CMaterial* material = GetOwner()->GetMeshRenderer()->GetMaterial();
    if (!material) return;

    if (m_bIsPressed && m_PressedTex)
    {
        material->SetTexture(0, m_PressedTex);
    }
    else if (m_bIsHovered && m_HoverTex)
    {
        material->SetTexture(0, m_HoverTex);
    }
    else if (m_DefaultTex)
    {
        material->SetTexture(0, m_DefaultTex);
    }
}