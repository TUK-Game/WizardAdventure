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
#include "AdcBall.h"
#include "RigidBody.h"
#include "Layer.h"

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
    m_bDoAttack = false;

    m_RangeCircle = new CAttackRangeCircle;
    Vec3 offset = -entity->GetTransform()->GetWorldDir(EDir::Front) * 400.f;
    m_RangeCircle->GetTransform()->SetRelativePosition(entity->GetTransform()->GetRelativePosition() + offset);
    m_RangeCircle->SetDuration(m_AttackDuration * 0.5122);
    m_RangeCircle->SetScaleRange(Vec3(0.1f, 0.1f, 0.1f), Vec3(300.f, 300.f, 10.f));
    CLevelManager::GetInst()->GetCurrentLevel()->SafeAddGameObject(m_RangeCircle, LAYER_PROJECTILE, false);


    Vec3 front = entity->GetTransform()->GetWorldDir(EDir::Front);
    front.Normalize();

    float yaw = atan2f(front.x, front.z); // 라디안
    float yawDegree = XMConvertToDegrees(yaw); // 디그리로 변환

    m_RangeRect = new CAttackRangeRect;

    m_RangeRect->GetTransform()->SetRelativePosition(entity->GetTransform()->GetRelativePosition());

    m_RangeRect->GetTransform()->SetRelativeRotation(90.f, yawDegree, 0.f);

    m_RangeRect->SetScaleRange(Vec3(70.f, 0.1f, 0.1f), Vec3(70.f, 800.f, 1.f));
    m_AttackTime = m_AttackDuration * 0.5122;
    m_RangeRect->SetDuration(m_AttackTime);
    m_RangeRect->SetInitialPosition(entity->GetTransform()->GetRelativePosition());


    CLevelManager::GetInst()->GetCurrentLevel()->SafeAddGameObject(m_RangeRect, LAYER_PROJECTILE, false);

}

void CMonsterAttackState::Update(CGameObject* entity, float deltaTime)
{
    m_ElapsedTime += deltaTime;

    if (m_ElapsedTime >= m_AttackTime) {
        if (!m_bDoAttack) {
            Vec3 spawnPos = entity->GetTransform()->GetWorldPosition();
            Vec3 AttackDir = -entity->GetTransform()->GetWorldDir(EDir::Front);
            AttackDir.Normalize();

            CAdcBall* adcbBall = new CAdcBall();
            Vec3 offset = AttackDir * 50.f + Vec3(0.f, 130.f, 0.f);
            adcbBall->GetTransform()->SetRelativePosition(spawnPos + offset);
            Vec3 velocity = AttackDir * 2000.f;
            adcbBall->GetRigidBody()->SetVelocity(velocity);

            CLevelManager::GetInst()->GetCurrentLevel()->SafeAddGameObject(adcbBall, LAYER_PROJECTILE, false);
            m_bDoAttack = true;
        }
    }

    //if (m_ElapsedTime >= m_AttackDuration)
    //    entity->GetStateManager()->HandleEvent(entity, "Idle");
}

void CMonsterAttackState::Exit(CGameObject* entity)
{
#ifdef _DEBUG;
    std::cout << "Exiting Monster Attack State" << std::endl;
#endif
    if (m_RangeCircle) {
        CLevelManager::GetInst()->GetCurrentLevel()->GetLayer(m_RangeCircle->GetLayerIndex())->SafeRemoveGameObject(m_RangeCircle);
    }

    if (m_RangeRect) {
        CLevelManager::GetInst()->GetCurrentLevel()->GetLayer(m_RangeRect->GetLayerIndex())->SafeRemoveGameObject(m_RangeRect);
    }
}