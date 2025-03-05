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
    bool isMouseOver = IsMouseOver(); // ���콺�� ��ư ���� �ִ��� Ȯ��

    // ���콺�� ��ư ���� ó�� ������ �� �� ���� ����
    if (isMouseOver && !m_bIsHovered)
    {
        CAssetManager::GetInst()->SoundPlay("Click"); // ȣ�� ���� ���
        m_bIsHovered = true; // ȣ���� ���� ������Ʈ
    }
    else if (!isMouseOver) // ���콺�� ����� �ٽ� �ʱ�ȭ
    {
        m_bIsHovered = false;
    }

    // ���콺 Ŭ�� ó��
    static bool isClicked = false;
    if (isMouseOver)
    {
        if ((GetAsyncKeyState(VK_LBUTTON) & 0x8000) && !isClicked)
        {
            CAssetManager::GetInst()->SoundPlay("Click");
            isClicked = true;

            if (onClick)
                onClick(); // ��ư Ŭ�� �̺�Ʈ ����
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