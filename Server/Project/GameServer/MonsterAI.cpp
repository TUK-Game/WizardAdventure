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

void CMonsterAI::Update(float deltaTime)
{
	DetectTarget();  // 타겟 탐색
	UpdateAI(deltaTime);      // 상태 전이 결정
}

void CMonsterAI::RotateToTarget(float deltaTime)
{
    const auto& myPosInfo = m_Owner->MonsterInfo->mutable_object_info()->mutable_pos_info();
    float currentY = myPosInfo->rotation().y();
    float targetY = m_TargetAngle;

    // 최소 회전 각도로 보정
    float diff = targetY - currentY;
    while (diff > 180.f) diff -= 360.f;
    while (diff < -180.f) diff += 360.f;

    float rotationSpeed = 180.f; // 도/초
    float deltaAngle = rotationSpeed * deltaTime;

    if (fabsf(diff) < 0.01f)
    {
        currentY = targetY;
    }
    else if (fabs(diff) < deltaAngle)
        currentY = targetY;
    else
        currentY += (diff > 0 ? deltaAngle : -deltaAngle);

    m_Owner->MonsterInfo->mutable_object_info()->mutable_pos_info()->mutable_rotation()->set_y(currentY);
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
    const auto& players = g_Room->GetPlayers();
    const auto& myPos = m_Owner->MonsterInfo->mutable_object_info()->mutable_pos_info()->position();
    Vec3 myPosition = Vec3(myPos.x(), myPos.y(), myPos.z());

    float closestDistance = FLT_MAX;

    for (const auto& player : players)
    {
        if (!player)
            continue;

        const auto& playerPos = player->PlayerInfo->mutable_object_info()->mutable_pos_info()->position();
        Vec3 playerPosition = Vec3(playerPos.x(), playerPos.y(), playerPos.z());

        float distance = (playerPosition - myPosition).Length();

        if (distance < closestDistance)
        {
            closestDistance = distance;
            m_Target = player;
        }
    }
}

void CMonsterAI::UpdateAI(float deltaTime)
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

    if ((distance <= m_AttackRange && IsFacingTarget(20.f)) || m_Owner->GetState() == Protocol::MOVE_STATE_SKILL_Q)
    {
        m_AttackTime += deltaTime;
        if (m_AttackTime >= m_AttackDuration)
        {
            m_AttackTime = 0;
            m_Owner->SetState(Protocol::MOVE_STATE_IDLE);
            return;
        }
        m_Owner->SetState(Protocol::MOVE_STATE_SKILL_Q);
    }
    else if (distance <= m_DetectRange)
    {
        m_Owner->SetState(Protocol::MOVE_STATE_RUN);
        Chase(deltaTime);
    }
    else
    {
        m_Owner->SetState(Protocol::MOVE_STATE_IDLE);
    }
}

void CMonsterAI::Chase(float deltaTime)
{
    if (!m_Target) return;

    RotateToTarget(deltaTime);

    const auto& myPos = m_Owner->MonsterInfo->mutable_object_info()->mutable_pos_info()->position();
    const auto& targetPos = m_Target->PlayerInfo->mutable_object_info()->mutable_pos_info()->position();
    Vec3 myPosition = Vec3(myPos.x(), myPos.y(), myPos.z());
    Vec3 targetPosition = Vec3(targetPos.x(), targetPos.y(), targetPos.z());

    Vec3 dir = targetPosition - myPosition;
    float dist = dir.Length();


    if (dist < 1.f) return; // 너무 가까우면 이동 생략
    dir.Normalize();

    Vec3 nextPos = myPosition + dir * 300.f * deltaTime;
    m_Owner->MonsterInfo->mutable_object_info()->mutable_pos_info()->mutable_position()->set_x(nextPos.x);
    m_Owner->MonsterInfo->mutable_object_info()->mutable_pos_info()->mutable_position()->set_y(nextPos.y);
    m_Owner->MonsterInfo->mutable_object_info()->mutable_pos_info()->mutable_position()->set_z(nextPos.z);
}
