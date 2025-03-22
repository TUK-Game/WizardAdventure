#include "pch.h"
#include "MonsterAI.h"
#include "GameObject.h"
#include "Transform.h"
#include "StateManager.h"
#include "LevelManager.h"

CMonsterAI::CMonsterAI()
    : CComponent(EComponent_Type::MonsterAI)
{
}

void CMonsterAI::Begin()
{
}

void CMonsterAI::Update()
{
    DetectTarget();  // 타겟 탐색
    UpdateAI();      // FSM 상태 전이 결정
}

void CMonsterAI::FinalUpdate()
{
}

void CMonsterAI::DetectTarget()
{
    // 플레이어 타겟 설정(임시)
    m_Target = CLevelManager::GetInst()->GetPlayer();
}

void CMonsterAI::UpdateAI()
{
    if (!m_Target) return;

    Vec3 myPos = GetTransform()->GetWorldPosition();
    Vec3 targetPos = m_Target->GetTransform()->GetWorldPosition();

    Vec3 direction = Vec3(targetPos.x - myPos.x, 0, targetPos.z - myPos.z);
    direction.Normalize();
    float angle = atan2(direction.x, direction.z) * (180.0f / XM_PI); // 라디안 → 도 단위 변환
    GetTransform()->SetRelativeRotation(0.f, angle + 180.f, 0.f);

    float distance = (targetPos - myPos).Length();

    CStateManager* sm = GetStateManager();
    if (!sm) return;

    /*if (distance <= m_AttackRange)
    {
        sm->HandleEvent(GetOwner(), "Attack");
    }
    else */if (distance <= m_DetectRange)
    {
        sm->HandleEvent(GetOwner(), "Chase");
    }
    else
    {
        sm->HandleEvent(GetOwner(), "Idle");
    }
}

