#include "pch.h"
#include "PlayerKnockbackState.h"
#include "Player.h"
#include "Animator.h"
#include "StateManager.h"
#include "NetworkManager.h"
#include "Transform.h"

void CPlayerKnockbackState::Enter(CGameObject* entity)
{
    CPlayer* player = dynamic_cast<CPlayer*>(entity);
    if (!player) return;

#ifdef _DEBUG
    std::cout << "Entering Knockback State" << std::endl;
#endif

    m_KnockbackDir = -player->GetCurrentMoveDir();
    m_Elapsed = 0.f;
    std::vector<CGameObject*> objs = player->GetChild();
    for (const auto o : objs) {
        CAnimator* ani = o->GetAnimator();
        ani->Play(L"KNOCKBACK"); 
        m_Duration = ani->GetDuration();
        std::cout << m_Duration << '\n';
    }
}

void CPlayerKnockbackState::Update(CGameObject* entity, float deltaTime)
{
   /* CPlayer* player = dynamic_cast<CPlayer*>(entity);
    if (!player) return;

    m_Elapsed += deltaTime;

    Vec3 front = player->GetTransform()->GetWorldDir(EDir::Front);
    front.Normalize();

    player->Move(front, false, 0.1f);

    if (m_Elapsed >= m_Duration)
    {
        player->GetStateManager()->HandleEvent(player, "EndKnockback");
    }*/
}

void CPlayerKnockbackState::Exit(CGameObject* entity)
{
#ifdef _DEBUG
    std::cout << "Exiting Knockback State" << std::endl;
#endif
}