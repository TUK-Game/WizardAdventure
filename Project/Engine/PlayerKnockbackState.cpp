#include "pch.h"
#include "PlayerKnockbackState.h"
#include "Player.h"
#include "Animator.h"
#include "StateManager.h"
#include "NetworkManager.h"
#include "Player.h"
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
    }
}

void CPlayerKnockbackState::Update(CGameObject* entity, float deltaTime)
{
    CPlayer* player = dynamic_cast<CPlayer*>(entity);
    if (!player) return;

    m_Elapsed += deltaTime;

    Vec3 knockbackMove = m_KnockbackDir * player->GetStats()->moveSpeed * 0.5f * deltaTime;
    player->GetTransform()->SetRelativePosition(
        player->GetTransform()->GetRelativePosition() + knockbackMove
    );

    if (m_Elapsed >= m_Duration)
    {
        player->GetStateManager()->HandleEvent(player, "EndKnockback");
    }
}

void CPlayerKnockbackState::Exit(CGameObject* entity)
{
#ifdef _DEBUG
    std::cout << "Exiting Knockback State" << std::endl;
#endif
}