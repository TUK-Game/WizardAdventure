#include "pch.h"
#include "MonsterAI.h"
#include "Monster.h"
#include "Player.h"
#include "Room.h"

CMonsterAI::CMonsterAI()
{
}

CMonsterAI::~CMonsterAI()
{
}

void CMonsterAI::Update()
{
	DetectTarget();  // 타겟 탐색
	UpdateAI();      // 상태 전이 결정
}

void CMonsterAI::RotateToTarget(float deltaTime)
{
}

bool CMonsterAI::IsFacingTarget(float angleThresholdDeg)
{
    const auto& currentPosInfo = m_Owner->MonsterInfo->mutable_object_info()->mutable_pos_info();

    float currentAngle = currentPosInfo->rotation().y();

    const auto& myPos = m_Owner->MonsterInfo->mutable_object_info()->mutable_pos_info()->position();
    const auto& targetPos = m_Target->PlayerInfo->mutable_object_info()->mutable_pos_info()->position();
    Vec3 myPosition = Vec3(myPos.x(), myPos.y(), myPos.z());
    Vec3 targetPosition = Vec3(targetPos.x(), targetPos.y(), targetPos.z());

    Vec3 toTarget = targetPosition - myPosition;
    toTarget.y = 0.f;

    if (toTarget.Length() < 0.001f)
        return true; // 너무 가까우면 그냥 true

    toTarget.Normalize();
    float targetAngle = atan2(toTarget.x, toTarget.z) * (180.0f / XM_PI) + 180.f;

    float diff = targetAngle - currentAngle;

    // -180 ~ 180 범위로 정규화
    if (diff > 180.f) diff -= 360.f;
    else if (diff < -180.f) diff += 360.f;

    return fabs(diff) <= angleThresholdDeg;
}

void CMonsterAI::DetectTarget()
{
}

void CMonsterAI::UpdateAI()
{
	if (!m_Target) 
		return;
	const auto& myPos = m_Owner->MonsterInfo->mutable_object_info()->mutable_pos_info()->position();
	const auto& targetPos = m_Target->PlayerInfo->mutable_object_info()->mutable_pos_info()->position();
	Vec3 myPosition = Vec3(myPos.x(), myPos.y(), myPos.z());
	Vec3 targetPosition = Vec3(targetPos.x(), targetPos.y(), targetPos.z());

    Vec3 dir = targetPosition - myPosition;
    dir.y = 0.f;
    if (dir.Length() > 0.001f)
    {
        dir.Normalize();
        float angle = atan2(dir.x, dir.z) * (180.0f / XM_PI);
        m_TargetAngle = angle + 180.f;
    }

    float distance = (targetPosition - myPosition).Length();

    //CStateManager* sm = GetStateManager();
    //if (!sm) return;

    //if (distance <= m_AttackRange && IsFacingTarget(20.f))
    //{
    //    sm->HandleEvent(GetOwner(), "Attack");
    //}
    //else if (distance <= m_DetectRange)
    //{
    //    sm->HandleEvent(GetOwner(), "Chase");
    //}
    //else
    //{
    //    sm->HandleEvent(GetOwner(), "Idle");
    //}
}
