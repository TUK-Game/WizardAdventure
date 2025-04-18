#pragma once
#include "GameObject.h"

class CPlayer;

class CSkillObject : public CGameObject
{
public:
    void SetDamage(int dmg) { m_Damage = dmg; }
    void SetCaster(CPlayer* caster) { m_Caster = caster; }

    int GetDamage() const { return m_Damage; }
    CPlayer* GetCaster() const { return m_Caster; }

    virtual void Update() override;
    virtual void FinalUpdate() override;

    virtual void OnHit(CGameObject* target); 
    virtual void CollisionBegin(CBaseCollider* src, CBaseCollider* dest) override;

    UINT64 m_ProjectileId{};
    bool m_bDelete{ false };
protected:
    int m_Damage = 0;
    CPlayer* m_Caster = nullptr;
};