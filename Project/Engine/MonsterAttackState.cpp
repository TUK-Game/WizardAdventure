#include "pch.h"
#include "MonsterAttackState.h"
#include "Animator.h"
#include "MonsterAI.h"
#include "Transform.h"
#include "StateManager.h"
#include "AttackRangeCircle.h"
#include "LevelManager.h"
#include "Level.h"

void CMonsterAttackState::Enter(CGameObject* entity)
{
#ifdef _DEBUG;
	std::cout << "Entering Monster Attack State" << std::endl;
#endif
    std::vector<CGameObject*> objs = entity->GetChild();
    for (const auto o : objs) {
        auto ani = o->GetAnimator();
        if (ani) {
            ani->Play(L"MONSTERATTACK1");
            m_AttackDuration = ani->GetDuration();
        }


    }
    m_ElapsedTime = 0.f;


    CAttackRangeCircle* attackRangeCircle = new CAttackRangeCircle;
    Vec3 offset = -entity->GetTransform()->GetWorldDir(EDir::Front) * 400.f;
    std::cout << "DIR: " << -entity->GetTransform()->GetWorldDir(EDir::Front).x << " " << -entity->GetTransform()->GetWorldDir(EDir::Front).y << " " << -entity->GetTransform()->GetWorldDir(EDir::Front).z << '\n';
    attackRangeCircle->GetTransform()->SetRelativePosition(entity->GetTransform()->GetRelativePosition() + offset);
    attackRangeCircle->SetDuration(m_AttackDuration * 0.5122);    
    attackRangeCircle->SetScaleRange(Vec3(0.1f, 0.1f, 0.1f), Vec3(300.f, 300.f, 10.f));
    CLevelManager::GetInst()->GetCurrentLevel()->SafeAddGameObject(attackRangeCircle, LAYER_PROJECTILE, false);
    std::cout << attackRangeCircle->GetTransform()->GetRelativePosition().x << " " << attackRangeCircle->GetTransform()->GetRelativePosition().y << " "
        << attackRangeCircle->GetTransform()->GetRelativePosition().z << "\n\n";
}

void CMonsterAttackState::Update(CGameObject* entity, float deltaTime)
{
    //m_ElapsedTime += deltaTime;
    //if (m_ElapsedTime >= m_AttackDuration)
    //    entity->GetStateManager()->HandleEvent(entity, "Idle");
}

void CMonsterAttackState::Exit(CGameObject* entity)
{
#ifdef _DEBUG;
    std::cout << "Exiting Monster Attack State" << std::endl;
#endif
}