#include "pch.h"
#include "PlayerScript.h"
#include "Player.h"
#include "StateManager.h"
#include "InputManager.h"
#include <iostream>

void CPlayerScript::Update()
{
    CPlayer* player = (CPlayer*)GetOwner();
    CStateManager* stateManager = player->GetStateManager();
    if (!stateManager) return;

    if (KEY_PUSH(EKey::Up))
        std::cout << "KeyUp" << std::endl;
        stateManager->ChangeState(player, EState_Type::Run);

}