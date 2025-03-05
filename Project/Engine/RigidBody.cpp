#include "pch.h"
#include "Light.h"
#include "Engine.h"
#include "Transform.h"
#include "RigidBody.h"
#include "GameObject.h"

CRigidBody::CRigidBody()
    : CComponent(EComponent_Type::Rigidbody), m_Velocity(0, 0, 0), m_Acceleration(0, 0, 0),
    m_AngularVelocity(0, 0, 0), m_Mass(1.0f), m_Drag(0.002f), m_AngularDrag(0.05f),  m_bUseGravity(false), m_bKinematic(false)
{
}

CRigidBody::~CRigidBody() {}

void CRigidBody::ApplyForce(const Vector3& force)
{
    if (m_bKinematic) return;

    m_Acceleration.x += force.x / m_Mass;
    m_Acceleration.y += force.y / m_Mass;
    m_Acceleration.z += force.z / m_Mass;
}

void CRigidBody::ApplyImpulse(const Vector3& impulse)
{
    if (m_bKinematic) return;

    m_Velocity.x += impulse.x / m_Mass;
    m_Velocity.y += impulse.y / m_Mass;
    m_Velocity.z += impulse.z / m_Mass;
}

void CRigidBody::ApplyTorque(const Vector3& torque)
{
    if (m_bKinematic) return;

    m_AngularVelocity.x += torque.x / m_Mass;
    m_AngularVelocity.y += torque.y / m_Mass;
    m_AngularVelocity.z += torque.z / m_Mass;
}


void CRigidBody::Begin()
{
    // 초기화 로직 (필요하면 추가)
}

void CRigidBody::Update()
{
    if (m_bKinematic) return;

    //m_Acceleration = { 0, 0, 0 };

    if (m_bUseGravity)
    {
        ApplyForce({ 0, -9.81f * m_Mass, 0 });   
    }

    // 속도 업데이트 
    m_Velocity.x += m_Acceleration.x * DELTA_TIME;  
    m_Velocity.y += m_Acceleration.y * DELTA_TIME;
    m_Velocity.z += m_Acceleration.z * DELTA_TIME;

    // 마찰력 적용 (속도를 점점 줄이기)
    m_Velocity.x *= (1.0f - m_Drag);
    m_Velocity.y *= (1.0f - m_Drag);
    m_Velocity.z *= (1.0f - m_Drag);

    m_AngularVelocity.x *= (1.0f - m_AngularDrag);
    m_AngularVelocity.y *= (1.0f - m_AngularDrag);
    m_AngularVelocity.z *= (1.0f - m_AngularDrag);

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

            // 회전 적용
            transform->SetRelativeRotation(
                transform->GetRelativeRotation().x + m_AngularVelocity.x * DELTA_TIME,
                transform->GetRelativeRotation().y + m_AngularVelocity.y * DELTA_TIME,
                transform->GetRelativeRotation().z + m_AngularVelocity.z * DELTA_TIME
            );
        }
    }

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
