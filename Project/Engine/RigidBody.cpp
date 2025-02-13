#include "pch.h"
#include "Light.h"
#include "Engine.h"
#include "Transform.h"
#include "RigidBody.h"
#include "GameObject.h"

CRigidBody::CRigidBody()
    : CComponent(EComponent_Type::Rigidbody), m_Velocity(0, 0, 0), m_Acceleration(0, 0, 0), m_Mass(1.0f), m_bUseGravity(false)
{
}

CRigidBody::~CRigidBody() {}

void CRigidBody::ApplyForce(const Vector3& force)
{
    m_Acceleration.x += force.x / m_Mass;
    m_Acceleration.y += force.y / m_Mass;
    m_Acceleration.z += force.z / m_Mass;
}

void CRigidBody::ApplyImpulse(const Vector3& impulse)
{
    m_Velocity.x += impulse.x / m_Mass;
    m_Velocity.y += impulse.y / m_Mass;
    m_Velocity.z += impulse.z / m_Mass;
}


void CRigidBody::Begin()
{
    // 초기화 로직 (필요하면 추가)
}

void CRigidBody::Update()
{
    if (m_bUseGravity)
    {
        ApplyForce({ 0, -9.81f, 0 });  // 중력 적용
    }

    m_Velocity.x += m_Acceleration.x * DELTA_TIME;  // deltaTime = 1/60초 기준
    m_Velocity.y += m_Acceleration.y * DELTA_TIME;
    m_Velocity.z += m_Acceleration.z * DELTA_TIME;

    // Transform 위치 갱신
    if (GetOwner())
    {
        CTransform* transform = GetTransform();
        if (transform)
        {
            transform->SetRelativePosition(
                transform->GetRelativePosition().x + m_Velocity.x * DELTA_TIME,
                transform->GetRelativePosition().y + m_Velocity.y * DELTA_TIME,
                transform->GetRelativePosition().z + m_Velocity.z * DELTA_TIME
            );
        }
    }

    m_Acceleration.x *= 0.98f;  // 감속 (공기 저항 효과)
    m_Acceleration.y *= 0.98f;
    m_Acceleration.z *= 0.98f;
}

void CRigidBody::FinalUpdate()
{
    // 충돌 감지 & 반응 추가 가능
    if (GetOwner())
    {
        CTransform* transform = GetTransform();
        if (transform->GetRelativePosition().y < 0.f)
        {
            transform->SetRelativePosition(transform->GetRelativePosition().x, 0.0f, transform->GetRelativePosition().z);

            // 속도 반전 (튕기는 효과)
            m_Velocity.y = -m_Velocity.y * 0.7f; // 0.7f는 탄성 계수 (1.0f이면 완전 탄성 충돌)

            // 아주 작은 값이면 멈추도록 처리 (멈추지 않으면 무한 진동할 수도 있음)
            if (fabs(m_Velocity.y) < 0.1f)
            {
                m_Velocity.y = 0.0f;
            }
        }
    }
}
