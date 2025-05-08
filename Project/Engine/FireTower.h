#pragma once
#include "SkillObject.h"
#include "AnimatedBillboardEffect.h"
#include "FireCircle.h"
#define FIRETOWER_ATTACK_RANGE 700


class CFireTower :
    public CSkillObject
{
public:
    CFireTower();

    void SetDuration(float duration) { m_Duration = duration; }
    void SetScaleDuration(float duration) { m_ScaleDuration = duration; }
    
    void UpdateScaleLerp();
    void TryAttack();
    void FireAtEnemy(CGameObject* enemy); // 적을 향해 파이어볼 발사

    void Init(CGameObject* owner);
    virtual void Update();
    virtual void FinalUpdate();
    virtual void ShowParticles();
    virtual void OffParticles();

private:
    CAnimatedBillboardEffect* m_FireEffect = nullptr;
    CFireCircle* m_FireCircle = nullptr;

    float m_Duration = 6.f;
    float m_ScaleDuration = 6.f;
    float m_ElapsedTime = 0.f;
    bool m_bFinishScale = false;

    float m_DissolveDuration = 1.f;



    bool m_bCanAttack = false;
    float m_AttackCooldown = 1.f;    // 공격 쿨타임
    float m_TimeSinceLastAttack = 0.f;
    float m_AttackRange = FIRETOWER_ATTACK_RANGE;
    
};

