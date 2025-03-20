#include "pch.h"
#include "PlayerScript.h"
#include "Player.h"
#include "StateManager.h"
#include "InputManager.h"
#include "Animator.h"
#include <iostream>

void CPlayerScript::Update()
{
    CPlayer* player = (CPlayer*)GetOwner();

    CStateManager* stateManager = player->GetStateManager();
    if (!stateManager) return;

    Vec3 moveDir = Vec3(0, 0, 0);
    if (KEY_PUSH(EKey::Up))    moveDir.z += 1;
    if (KEY_PUSH(EKey::Down))  moveDir.z -= 1;
    if (KEY_PUSH(EKey::Left))  moveDir.x -= 1;
    if (KEY_PUSH(EKey::Right)) moveDir.x += 1;

    if (moveDir.Length() > 0)
    {
        stateManager->HandleEvent(player, "Move");  // 이동 이벤트 전달
    }
}