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
private:
    Vec3 m_currentMoveDir;
    EPlayerAttribute m_Attribute;  // 플레이어 속성
    CSkillManager* m_SkillManager; // 플레이어 스킬 관리
};

