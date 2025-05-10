#include "pch.h"
#include "PlayerFallingState.h"
#include "GameObject.h"
#include "Animator.h"
#include "Player.h"

void CPlayerFallingState::Enter(CGameObject* entity)
{
    CPlayer* player = dynamic_cast<CPlayer*>(entity);
    if (!player) return;

    std::vector<CGameObject*> objs = player->GetChild();
    for (const auto o : objs) {
        CAnimator* ani = o->GetAnimator();
        ani->Play(L"FALLING");
        std::cout << ani->GetDuration() << '\n';
    }
}

void CPlayerFallingState::Update(CGameObject* entity, float deltaTime)
{

}

void CPlayerFallingState::Exit(CGameObject* entity)
{
}