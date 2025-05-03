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

    if (stateManager->GetCurrentStateType() == EState_Type::Death)
        return;

    if (player->GetStats()->currentHp <= 0)
    {
        stateManager->HandleEvent(player, "Death");
        return;
    }

    if (m_bShopping)
    {
        if (KEY_DOWN(EKey::G))
        {
            m_bShopping = false;
            player->FinishShopping();
        }
        return;
    }

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
    if (KEY_DOWN(EKey::F))
    {
        player->DetectNPC();
        m_bShopping = true;
    }


    // temp ----------------------------------------------------------------------------
    if (KEY_DOWN(EKey::Z))
    {
        stateManager->HandleEvent(player, "Knockback");
    }
    if (KEY_DOWN(EKey::X))
    {
        stateManager->HandleEvent(player, "Death");
    }
    // temp ----------------------------------------------------------------------------
}