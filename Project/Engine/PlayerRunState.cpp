#include "pch.h"
#include "PlayerRunState.h"
#include "Player.h"
#include "Animator.h"
#include "InputManager.h"
#include "StateManager.h"

void CPlayerRunState::Enter(CGameObject* entity)
{
	std::cout << "Entering Run State" << std::endl;
    std::vector<CGameObject*> objs = entity->GetChild();
    for (const auto o : objs) {
        o->GetAnimator()->Play(1);
    }
}

void CPlayerRunState::Update(CGameObject* entity, float deltaTime)
{
    CPlayer* player = dynamic_cast<CPlayer*>(entity);

    Vec3 moveDir = Vec3(0, 0, 0);
    if (KEY_PUSH(EKey::Up))    moveDir.z += 1;
    if (KEY_PUSH(EKey::Down))  moveDir.z -= 1;
    if (KEY_PUSH(EKey::Left))  moveDir.x -= 1;
    if (KEY_PUSH(EKey::Right)) moveDir.x += 1;

    if (moveDir.Length() > 0)
    {
        moveDir.Normalize();
        player->Move(moveDir);
    }
    else
    {
        // 입력이 없으면 Idle 상태로 전환
        player->GetStateManager()->HandleEvent(player, "Stop");
    }
}

void CPlayerRunState::Exit(CGameObject* entity)
{
    std::cout << "Exiting Run State" << std::endl;
}