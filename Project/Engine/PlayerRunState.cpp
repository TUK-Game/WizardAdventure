#include "pch.h"
#include "PlayerRunState.h"
#include "Player.h"
#include "Animator.h"
#include "InputManager.h"
#include "StateManager.h"
#include "LevelManager.h"
#include "ServerSession.h"
#include "NetworkManager.h"
#include "AssetManager.h"

void CPlayerRunState::Enter(CGameObject* entity)
{
#ifdef _DEBUG;
	std::cout << "Entering Run State" << std::endl;
#endif
    std::vector<CGameObject*> objs = entity->GetChild();
    for (const auto o : objs) {
        if(o->GetAnimator())
            o->GetAnimator()->Play(L"RUNNING");
    }

}

void CPlayerRunState::Update(CGameObject* entity, float deltaTime)
{
    CPlayer* player = dynamic_cast<CPlayer*>(entity);

    if (CLevelManager::GetInst()->GetOwnPlayer() != player)
        return;

    Vec3 moveDir = Vec3(0, 0, 0);
    if (KEY_PUSH(EKey::W))    moveDir.z += 1;
    if (KEY_PUSH(EKey::S))  moveDir.z -= 1;
    if (KEY_PUSH(EKey::A))  moveDir.x -= 1;
    if (KEY_PUSH(EKey::D)) moveDir.x += 1;

    if (moveDir.Length() > 0)
    {
        moveDir.Normalize();
        player->Move(moveDir);
    }
    else
    {
        // �Է��� ������ Idle ���·� ��ȯ
        player->GetStateManager()->HandleEvent(player, "Stop");
    }
}

void CPlayerRunState::Exit(CGameObject* entity)
{
#ifdef _DEBUG;
    std::cout << "Exiting Run State" << std::endl;
#endif

}