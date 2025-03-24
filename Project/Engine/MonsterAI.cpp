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
    DetectTarget();  // Ÿ�� Ž��
    UpdateAI();      // FSM ���� ���� ����
}

void CMonsterAI::FinalUpdate()
{
}

void CMonsterAI::DetectTarget()
{
    // �÷��̾� Ÿ�� ����(�ӽ�)
    m_Target = CLevelManager::GetInst()->GetPlayer();
}

void CMonsterAI::UpdateAI()
{
    if (!m_Target) return;

    Vec3 myPos = GetTransform()->GetWorldPosition();
    Vec3 targetPos = m_Target->GetTransform()->GetWorldPosition();

    Vec3 dir = targetPos - myPos;
    dir.y = 0.f;
    if (dir.Length() > 0.001f)
    {
        dir.Normalize();
        float angle = atan2(dir.x, dir.z) * (180.0f / XM_PI);
        m_TargetAngle = angle + 180.f;
    }

    float distance = (targetPos - myPos).Length();

    CStateManager* sm = GetStateManager();
    if (!sm) return;

    if (distance <= m_AttackRange && IsFacingTarget(20.f))
    {
        sm->HandleEvent(GetOwner(), "Attack");
    }
    else if (distance <= m_DetectRange)
    {
        sm->HandleEvent(GetOwner(), "Chase");
    }
    else
    {
        sm->HandleEvent(GetOwner(), "Idle");
    }
}

void CMonsterAI::RotateToTarget(float deltaTime)
{
    CTransform* transform = GetTransform();

    float currentY = transform->GetRelativeRotation().y;
    float targetY = m_TargetAngle;

    // �ּ� ȸ�� ������ ����
    float diff = targetY - currentY;
    if (diff > 180.f) diff -= 360.f;
    else if (diff < -180.f) diff += 360.f;

    float rotationSpeed = 180.f; // ��/��
    float deltaAngle = rotationSpeed * deltaTime;

    if (fabs(diff) < deltaAngle)
        currentY = targetY;
    else
        currentY += (diff > 0 ? deltaAngle : -deltaAngle);

    Vec3 rot = transform->GetRelativeRotation();
    rot.y = currentY;
    transform->SetRelativeRotation(rot);
}

bool CMonsterAI::IsFacingTarget(float angleThresholdDeg) 
{
    CTransform* transform = GetTransform();
    if (!transform || !m_Target) return false;

    float currentAngle = transform->GetRelativeRotation().y;

    Vec3 myPos = transform->GetWorldPosition();
    Vec3 targetPos = m_Target->GetTransform()->GetWorldPosition();

    Vec3 toTarget = targetPos - myPos;
    toTarget.y = 0.f;

    if (toTarget.Length() < 0.001f)
        return true; // �ʹ� ������ �׳� true

    toTarget.Normalize();
    float targetAngle = atan2(toTarget.x, toTarget.z) * (180.0f / XM_PI) + 180.f;

    float diff = targetAngle - currentAngle;

    // -180 ~ 180 ������ ����ȭ
    if (diff > 180.f) diff -= 360.f;
    else if (diff < -180.f) diff += 360.f;

    return fabs(diff) <= angleThresholdDeg;
}