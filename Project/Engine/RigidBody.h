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


    Vector3 GetVelocity() const { return m_Velocity; }
    Vector3 GetAngularVelocity() const { return m_AngularVelocity; }


public:
    virtual void Begin() override;
    virtual void Update() override;
    virtual void FinalUpdate() override;
    virtual CRigidBody* Clone() override { return new CRigidBody(*this); }

private:
    Vector3 m_Velocity;         // 선속도 (m/s)
    Vector3 m_Acceleration;     // 가속도 (m/s²)
    Vector3 m_AngularVelocity;  // 각속도 (rad/s)

    float m_Mass;               // 질량 (kg)
    float m_Drag;               // 선형 마찰 계수
    float m_AngularDrag;        // 회전 마찰 계수
    bool m_bUseGravity;         // 중력 사용 여부
    bool m_bKinematic;          // Kinematic 상태 여부
};
