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
    void SetVelocity(const Vector3& vel) { m_Velocity = vel; }
    void SetGravity(const bool b) { m_bUseGravity = b; }

    Vector3 GetVelocity() const { return m_Velocity; }

public:
    virtual void Begin() override;
    virtual void Update() override;
    virtual void FinalUpdate() override;
    virtual CRigidBody* Clone() override { return new CRigidBody(*this); }

private:
    Vector3 m_Velocity;
    Vector3 m_Acceleration;
    float m_Mass;
    bool m_bUseGravity;
};
