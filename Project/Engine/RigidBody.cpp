#include "pch.h"
#include "Light.h"
#include "Engine.h"
#include "Transform.h"
#include "RigidBody.h"
#include "GameObject.h"

CRigidBody::CRigidBody()
    : CComponent(EComponent_Type::Rigidbody), m_Velocity(0, 0, 0), m_Acceleration(0, 0, 0),
    m_AngularVelocity(0, 0, 0), m_Mass(1.0f), m_Drag(0.002f), m_AngularDrag(0.05f),  m_bUseGravity(false), m_bKinematic(true)
{
}

CRigidBody::~CRigidBody() {}

void CRigidBody::ApplyForce(const Vector3& force)
{
    if (!m_bKinematic) return;

    m_Acceleration.x += force.x / m_Mass;
    m_Acceleration.y += force.y / m_Mass;
    m_Acceleration.z += force.z / m_Mass;
}   

void CRigidBody::ApplyImpulse(const Vector3& impulse)
{
    if (!m_bKinematic) return;

    m_Velocity.x += impulse.x / m_Mass;
    m_Velocity.y += impulse.y / m_Mass;
    m_Velocity.z += impulse.z / m_Mass;
}

void CRigidBody::ApplyTorque(const Vector3& torque)
{
    if (!m_bKinematic) return;

    m_AngularVelocity.x += torque.x / m_Mass;
    m_AngularVelocity.y += torque.y / m_Mass;
    m_AngularVelocity.z += torque.z / m_Mass;
}


void CRigidBody::Begin()
{
    // �ʱ�ȭ ���� (�ʿ��ϸ� �߰�)
}

void CRigidBody::Update()
{
    if (!m_bKinematic) return;

    // �߷� ���� (�׻� �������� ��)
    if (m_bUseGravity)
    {
        ApplyForce({ 0, -981.f * m_Mass, 0 }); 
    }

    // Newton's Second Law: F = m * a -> a = F / m
    Vector3 totalAcceleration = m_Acceleration / m_Mass;

    // �ӵ� ������Ʈ (Verlet Integration ���)
    m_Velocity += totalAcceleration * DELTA_TIME;

    // ���� ���� (�ӵ��� ���������� ����)
    m_Velocity *= (1.0f - m_Drag);
    m_AngularVelocity *= (1.0f - m_AngularDrag);

    // Transform ��ġ ���� (���� �)
    if (GetOwner())
    {
        CTransform* transform = GetTransform();
        if (transform)
        {
            transform->SetRelativePosition(
                transform->GetRelativePosition() + m_Velocity * DELTA_TIME
            );

            // ȸ�� ����
            transform->SetRelativeRotation(
                transform->GetRelativeRotation() + m_AngularVelocity * DELTA_TIME
            );
        }
    }

    // ���� �������� ���� ���ӵ� �ʱ�ȭ (�ܺο��� ���ο� ���� �����ؾ� ��)
    m_Acceleration = { 0, 0, 0 };
}

void CRigidBody::FinalUpdate()
{
    //// �浹 ���� & ���� �߰� ����
    //if (GetOwner())
    //{
    //    CTransform* transform = GetTransform();
    //    if (transform->GetRelativePosition().y < 0.f)
    //    {
    //        transform->SetRelativePosition(transform->GetRelativePosition().x, 0.0f, transform->GetRelativePosition().z);

    //        // �ӵ� ���� (ƨ��� ȿ��)
    //        m_Velocity.y = -m_Velocity.y * 0.7f; // 0.7f�� ź�� ��� (1.0f�̸� ���� ź�� �浹)

    //        // ���� ���� ���̸� ���ߵ��� ó�� (������ ������ ���� ������ ���� ����)
    //        if (fabs(m_Velocity.y) < 0.1f)
    //        {
    //            m_Velocity.y = 0.0f;
    //        }
    //    }
    //}
}
