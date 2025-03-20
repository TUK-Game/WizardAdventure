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

    if (KEY_PUSH(EKey::Up) || KEY_PUSH(EKey::Down) || KEY_PUSH(EKey::Left) || KEY_PUSH(EKey::Right))
    {
        stateManager->HandleEvent(player, "Move");
    }

    if (KEY_DOWN(EKey::Space))
    {
        stateManager->HandleEvent(player, "Dash");
    }
}