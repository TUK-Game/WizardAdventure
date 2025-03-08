#pragma once

#include "Component.h"

class CRigidBody : public CComponent
{
public:
    CRigidBody();
    virtual ~CRigidBody();

public:
    void ApplyForce(const Vector3& force);
    void ApplyImpulse(const Vector3& impulse);
    void ApplyTorque(const Vector3& torque);

    void SetVelocity(const Vector3& vel) { m_Velocity = vel; }
    void SetAngularVelocity(const Vector3& angVel) { m_AngularVelocity = angVel; }
    void SetGravity(bool enable) { m_bUseGravity = enable; }
    void SetKinematic(bool enable) { m_bKinematic = enable; }
    void SetMass(float mass) { m_Mass = mass; }
    void SetDrag(float drag) { m_Drag = drag; }
    void SetAngularDrag(float angularDrag) { m_AngularDrag = angularDrag; }

    bool GetGravity() { return m_bUseGravity; }
    bool GetKinematic() { return m_bKinematic; }
    Vector3 GetVelocity() const { return m_Velocity; }
    Vector3 GetAngularVelocity() const { return m_AngularVelocity; }
    float GetMass() { return m_Mass; }
    float GetDrag() { return m_Drag; }
    float GetAngularDrag() { return m_AngularDrag; }


public:
    virtual void Begin() override;
    virtual void Update() override;
    virtual void FinalUpdate() override;
    virtual CRigidBody* Clone() override { return new CRigidBody(*this); }

private:
    Vector3 m_Velocity;         // ���ӵ� (m/s)
    Vector3 m_Acceleration;     // ���ӵ� (m/s��)
    Vector3 m_AngularVelocity;  // ���ӵ� (rad/s)

    float m_Mass;               // ���� (kg)
    float m_Drag;               // ���� ���� ���
    float m_AngularDrag;        // ȸ�� ���� ���
    bool m_bUseGravity;         // �߷� ��� ����
    bool m_bKinematic;          // Kinematic ���� ����
};
