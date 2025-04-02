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

    Vec3 moveDir(0, 0, 0);
    if (KEY_PUSH(EKey::W))    moveDir.z += 1;
    if (KEY_PUSH(EKey::S))  moveDir.z -= 1;
    if (KEY_PUSH(EKey::A))  moveDir.x -= 1;
    if (KEY_PUSH(EKey::D)) moveDir.x += 1;


    if (moveDir.Length() > 0.01f)
    {
        stateManager->HandleEvent(player, "Move");
    }
    if (KEY_DOWN(EKey::Space))
    {
        stateManager->HandleEvent(player, "Dash");
    }
    if (KEY_DOWN(EKey::LButton))
    {
        stateManager->HandleEvent(player, "Attack_LButton");
    }
    if (KEY_DOWN(EKey::RButton))
    {
        stateManager->HandleEvent(player, "Attack_RButton");
    }
    if (KEY_DOWN(EKey::Q))
    {
        stateManager->HandleEvent(player, "Attack_Q");
    }
    if (KEY_DOWN(EKey::E))
    {
        stateManager->HandleEvent(player, "Attack_E");
    }
    if (KEY_DOWN(EKey::R))
    {
        stateManager->HandleEvent(player, "Attack_R");
    }
}