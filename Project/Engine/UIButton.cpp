#include "pch.h"
#include "Engine.h"
#include "Transform.h"
#include "UIButton.h"
#include "UI.h"
#include "GameObject.h"
#include "AssetManager.h"

CUIButton::CUIButton() : CComponent(EComponent_Type::UIButton)
{
}

CUIButton::~CUIButton()
{
}

void CUIButton::Update()
{
    bool isMouseOver = IsMouseOver(); // 마우스가 버튼 위에 있는지 확인

    // 마우스가 버튼 위로 처음 들어왔을 때 한 번만 실행
    if (isMouseOver && !m_bIsHovered)
    {
        CAssetManager::GetInst()->SoundPlay("Click"); // 호버 사운드 재생
        m_bIsHovered = true; // 호버링 상태 업데이트
    }
    else if (!isMouseOver) // 마우스가 벗어나면 다시 초기화
    {
        m_bIsHovered = false;
    }

    // 마우스 클릭 처리
    static bool isClicked = false;
    if (isMouseOver)
    {
        if ((GetAsyncKeyState(VK_LBUTTON) & 0x8000) && !isClicked)
        {
            CAssetManager::GetInst()->SoundPlay("Click");
            isClicked = true;

            if (onClick)
                onClick(); // 버튼 클릭 이벤트 실행
        }
        else if (!(GetAsyncKeyState(VK_LBUTTON) & 0x8000))
        {
            isClicked = false;
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