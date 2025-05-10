#include "pch.h"
#include "MonsterAI.h"
#include "Monster.h"
#include "Player.h"
#include "Room.h"
#include "BoxCollider.h"
#include "LevelCollision.h"
#include "ProjectilePool.h"

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

    m_Owner->SetDir(currentY);
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
    //m_Owner->SetDir(dir);

    if (dir.Length() > 0.001f)
    {
        dir.Normalize();
        float angle = atan2(dir.x, dir.z) * (180.0f / XM_PI);
        m_TargetAngle = angle + 180.f;
    }

    if (m_Owner->GetState() == Protocol::MOVE_STATE_SPAWN)  // spawn
    {
        m_SpawnTime += deltaTime;
        if (m_SpawnTime >= m_SpawnDuration)
        {
            // 현재 위치에서 y축으로 300만큼 +해야함
            Protocol::Vector3* pos = m_Owner->MonsterInfo->mutable_object_info()->mutable_pos_info()->mutable_position();
            pos->set_y(pos->y() + 150.f);

            m_SpawnTime = 0.f;
            m_Owner->SetState(Protocol::MOVE_STATE_IDLE);
        }
        return;
    }

    if (m_Owner->GetState() == Protocol::MOVE_STATE_DAMAGED)        // damaged
    {
        m_DamagedTime += deltaTime;
        if (m_DamagedTime >= m_DamagedDuration)
        {
            m_DamagedTime = 0.f;
            m_Owner->SetState(Protocol::MOVE_STATE_DAMAGED_END);        // endDamaged
        }
        return;
    }
    
    if (m_Owner->GetState() == Protocol::MOVE_STATE_DEATH)  // death
    {
        m_DeathTime += deltaTime;
        if (m_DeathTime + 0.1f >= m_DeathDuration)
        {
            m_DeathTime = 0.f; 
            // dissolve Start
            m_Owner->SetState(Protocol::MOVE_STATE_DISSOVE);
            m_bAttack = false;
            auto& objects = g_Room->GetLayerObjects((uint32)EObject_Type::Projectile);
            for (uint32 id : m_ProjectileIds)
            {
                g_pool->Release(std::dynamic_pointer_cast<CProjectile>(objects[id]));
                objects.erase(id);
            }
            m_ProjectileIds.clear();
            m_Owner->ClearProjectileIds();
        }
        return;
    }

    if (m_Owner->GetState() == Protocol::MOVE_STATE_DISSOVE)  // dissolve
    {
        m_DissolveTime += deltaTime;
        if (m_DissolveTime >= m_DissolveDuration)
        {
            m_DissolveTime = 0.f;
            m_Owner->SetState(Protocol::MOVE_STATE_NONE);
            g_Room->RemoveObject((uint32)EObject_Type::Monster, m_Owner->MonsterInfo->object_id());
        }
        return;
    } 

    float distance = (targetPosition - myPosition).Length();

    if ((distance <= m_AttackRange && IsFacingTarget(20.f)) || m_Owner->GetState() == Protocol::MOVE_STATE_SKILL_Q)
    {
        m_AttackTime += deltaTime;
        if (m_AttackTime >= m_AttackDuration)
        {
            m_AttackTime = 0;
            m_Owner->SetState(Protocol::MOVE_STATE_IDLE);
            m_bAttack = false;
            auto& objects = g_Room->GetLayerObjects((uint32)EObject_Type::Projectile);
            for (uint32 id : m_ProjectileIds)
            {
                g_pool->Release(std::dynamic_pointer_cast<CProjectile>(objects[id]));
                objects.erase(id);
            }
            m_ProjectileIds.clear();
            m_Owner->ClearProjectileIds();
            return;
        }
        if (!m_bAttack && m_AttackTime >= m_AttackSpawn)
        {
            m_Owner->SpawnAttackObject();
            m_ProjectileIds = m_Owner->GetProjectileIds();
            m_bAttack = true;
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

    HandleMoveMonster(deltaTime);
    //HandleGravityMonster(deltaTime);
}

void CMonsterAI::HandleMoveMonster(float deltaTime)
{
    const auto& myPos = m_Owner->MonsterInfo->mutable_object_info()->mutable_pos_info()->position();
    const auto& targetPos = m_Target->PlayerInfo->mutable_object_info()->mutable_pos_info()->position();
    Vec3 myPosition = Vec3(myPos.x(), myPos.y(), myPos.z());
    Vec3 targetPosition = Vec3(targetPos.x(), targetPos.y(), targetPos.z());

    Vec3 dir = targetPosition - myPosition;
    float dist = dir.Length();

    if (dist < 1.f) 
        return; 

    dir.y = 0.f;
    dir.Normalize();

    int step = 1;

    Protocol::Vector3& protoNow = *m_Owner->MonsterInfo->mutable_object_info()->mutable_pos_info()->mutable_position();
    XMFLOAT3 nowPos(protoNow.x(), protoNow.y(), protoNow.z());

    Vec3 moveAmount = dir * 300.f * deltaTime;

    moveAmount.x /= static_cast<float>(step);
    moveAmount.z /= static_cast<float>(step);

    for (int i = 1; i <= step; ++i)
    {
        nowPos.x += moveAmount.x;
        nowPos.z += moveAmount.z;

        ToProtoVector3(&protoNow, nowPos);

        m_Owner->GetCollider()->Update();

        if (g_Room->GetLevelCollision()->CollisionWithWall(m_Owner->GetCollider()))
        {
            nowPos.x -= moveAmount.x;
            nowPos.z -= moveAmount.z;

            ToProtoVector3(&protoNow, nowPos);
            break;
        }
    }

    m_Owner->MonsterInfo->mutable_object_info()->mutable_pos_info()->mutable_position()->set_x(nowPos.x);
    m_Owner->MonsterInfo->mutable_object_info()->mutable_pos_info()->mutable_position()->set_z(nowPos.z);
}

void CMonsterAI::HandleGravityMonster(float deltaTime)
{
    const auto& myPos = m_Owner->MonsterInfo->mutable_object_info()->mutable_pos_info()->position();
    Vec3 myPosition = Vec3(myPos.x(), myPos.y(), myPos.z());

    int step = 1;

    Protocol::Vector3& protoNow = *m_Owner->MonsterInfo->mutable_object_info()->mutable_pos_info()->mutable_position();
    XMFLOAT3 nowPos(protoNow.x(), protoNow.y(), protoNow.z());

    float moveAmount = -GRAVITY * deltaTime;

    for (int i = 1; i <= step; ++i)
    {
        nowPos.y += moveAmount;

        ToProtoVector3(&protoNow, nowPos);

        m_Owner->GetCollider()->Update();
        auto box = m_Owner->GetCollider();
        box->SetBoxHeight(0.f);
        if (g_Room->GetLevelCollision()->CollisionWithWall(box))
        {
            if (nowPos.y > -200.f)
            {
                nowPos.y = 0.f;
                ToProtoVector3(&protoNow, nowPos);
            }
            break;
        }
    }
    m_Owner->MonsterInfo->mutable_object_info()->mutable_pos_info()->mutable_position()->set_y(nowPos.y);
}
