#include "pch.h"
#include "MonsterAttackState.h"
#include "Animator.h"
#include "MonsterAI.h"
#include "Transform.h"
#include "StateManager.h"
#include "AttackRangeCircle.h"
#include "AttackRangeRect.h"
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
    attackRangeCircle->GetTransform()->SetRelativePosition(entity->GetTransform()->GetRelativePosition() + offset);
    attackRangeCircle->SetDuration(m_AttackDuration * 0.5122);    
    attackRangeCircle->SetScaleRange(Vec3(0.1f, 0.1f, 0.1f), Vec3(300.f, 300.f, 10.f));
    CLevelManager::GetInst()->GetCurrentLevel()->SafeAddGameObject(attackRangeCircle, LAYER_PROJECTILE, false);


    Vec3 front = entity->GetTransform()->GetWorldDir(EDir::Front);
    front.Normalize();

    float yaw = atan2f(front.x, front.z); // 라디안
    float yawDegree = XMConvertToDegrees(yaw); // 디그리로 변환

    CAttackRangeRect* attackRangeRect = new CAttackRangeRect;

    attackRangeRect->GetTransform()->SetRelativePosition(entity->GetTransform()->GetRelativePosition());

    attackRangeRect->GetTransform()->SetRelativeRotation(90.f, yawDegree, 0.f);

    attackRangeRect->SetScaleRange(Vec3(100.f, 0.1f, 0.1f), Vec3(100.f, 500.f, 1.f));
    attackRangeRect->SetDuration(m_AttackDuration * 0.8f);
    attackRangeRect->SetInitialPosition(entity->GetTransform()->GetRelativePosition());


    CLevelManager::GetInst()->GetCurrentLevel()->SafeAddGameObject(attackRangeRect, LAYER_PROJECTILE, false);



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