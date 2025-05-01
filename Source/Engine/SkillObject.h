#pragma once
#include "GameObject.h"

class CPlayer;

class CSkillObject : public CGameObject
{
public:
    void SetDamage(int dmg) { m_Damage = dmg; }
    void SetCaster(CPlayer* caster);
    void SetCollisionExplosion(bool b) { m_bCollisionExplosion = b; }
    void SetIsSpawnParticle(bool b) { m_bSpawnParticle = b; }
    void SetSkillType(SKILL type) { m_type = type; }

    bool GetCollisionExplosion() { return m_bCollisionExplosion; }
    bool GetIsSpawnParticle() { return m_bSpawnParticle; }
    int GetDamage() const { return m_Damage; }
    CPlayer* GetCaster() const { return m_Caster; }
    SKILL GetSkillType() const { return m_type; }

    virtual void Update() override;
    virtual void FinalUpdate() override;

    virtual void OnHit(CGameObject* target); 
    virtual void CollisionBegin(CBaseCollider* src, CBaseCollider* dest) override;
    virtual void ShowParticles() {}
    virtual void OffParticles() {};

    UINT64 m_ProjectileId{};
    bool m_bDelete{ false };
protected:
    int m_Damage = 0;
    CPlayer* m_Caster = nullptr;
    SKILL m_type = SKILL::MAX;
    bool m_bOwn = { false };
    bool m_bCollisionExplosion{ false };
    bool m_bSpawnParticle{ false };
};