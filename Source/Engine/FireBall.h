#pragma once
#include "SkillObject.h"

enum class EFireBallMode
{
    Default,   
    QSkill,    
    Meteor     
};

class CFireBall :
    public CSkillObject
{
public:
    CFireBall();

    void SetDirection(const Vec3& dir) { m_Direction = dir; }
    void SetSpeed(float speed) { m_Speed = speed; }
    void SetDuration(float duration) { m_Duration = duration; }
    void UseSmokeTrail();
    void SpawnDeleteEffect();
        
    void SetFireParticleId(int id) { m_FireParticleId = id; }
    void SetSmokeParticleId(int id) { m_SmokeParticleId = id; }
    void SetMode(EFireBallMode mode) { m_Mode = mode; }
    EFireBallMode GetMode() const { return m_Mode; }
    void SetStartScale(const Vec3& scale) { m_StartScale = scale; }
    void SetEndScale(const Vec3& scale) { m_EndScale = scale; }

    int GetFireParticleId() { return m_FireParticleId; }
    int GetSmokeParticleId() { return m_SmokeParticleId; }
    bool GetIsBoom() { return m_bIsFireBallBoom; }

    void UpdateByMode();
    void UpdateScaleLerp();

    virtual void Update();
    virtual void FinalUpdate();

    virtual void CollisionBegin(CBaseCollider* src, CBaseCollider* dest) override;

private:
    int m_FireParticleId = -1;
    int m_SmokeParticleId = -1;
    Vec3 m_StartScale = Vec3(30.f, 30.f, 30.f);
    Vec3 m_EndScale = Vec3(5.f, 5.f, 5.f);
    Vec3 m_Direction;
    bool m_bIsFireBallBoom{};
    float m_Speed;
    float m_Duration = 0.3f;
    float m_ElapsedTime = 0.f;
    EFireBallMode m_Mode = EFireBallMode::Default;
};

