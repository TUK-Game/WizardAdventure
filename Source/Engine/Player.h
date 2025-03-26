#pragma once
#include "GameObject.h"
#include "SkillManager.h"

class CPlayer : public CGameObject
{
public:
    CPlayer(EPlayerAttribute attribute);
    virtual ~CPlayer();

    virtual void Begin();
    virtual void Update();
    virtual void FinalUpdate();
    virtual void Render();

    void Move(Vec3 moveDir, bool shouldRotate = true);
    void Attack(int skillIndex);

    void CreateStateManager();

    Vec3 GetCurrentMoveDir() { return m_currentMoveDir; }
    EPlayerAttribute GetAttribute() const { return m_Attribute; }

public:
    virtual void CollisionBegin(CBaseCollider* src, CBaseCollider* dest) override;

private:
    Vec3 m_currentMoveDir;
    EPlayerAttribute m_Attribute;  
    CSkillManager* m_SkillManager;
    float m_Speed = 1000.f;
};

