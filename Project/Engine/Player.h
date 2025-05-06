#pragma once
#include "GameObject.h"
#include "SkillManager.h"





class CPlayer : public CGameObject
{
public:
    CPlayer(EPlayerAttribute attribute, bool Owner = false);
    virtual ~CPlayer();

    virtual void Begin();
    virtual void Update();
    virtual void FinalUpdate();
    virtual void Render();

    void Move(Vec3 moveDir, bool shouldRotate = true);
    void Attack(int skillIndex, float duration = 0.f);

    virtual void SetTarget(const Vec3& pos, const Vec3& rot) { m_Interpolator->SetTarget(pos, rot); }
    void SetStats(int maxHp, int hp) { m_Stats->maxHp = maxHp; m_Stats->currentHp = hp; }
    void AddItem(std::shared_ptr<class CItem> item) { m_Items.emplace_back(item); }
    void CreateStateManager();

    Vec3 GetCurrentMoveDir() { return m_currentMoveDir; }
    EPlayerAttribute GetAttribute() const { return m_Attribute; }
    Stats* GetStats() const { return m_Stats; }
    CSkillManager* GetSkillManager() const { return m_SkillManager; }
    std::vector<std::shared_ptr<class CItem>>& GetItems() { return m_Items; }

    void InitStats(int maxHp, int hp, int attack, float speed);

    void DetectNPC();
    void MoveToInventoryView();
    void MoveCamera(CGameObject* camera, CGameObject* target, ECamera_Type type, const Vec3& offset);
    void FinishInteraction(bool isLayerCheck);
public:
    virtual void CollisionBegin(CBaseCollider* src, CBaseCollider* dest) override;
    Vec3 m_Amount;
    Vec3 m_NextPosition;

private:
    Vec3 m_currentMoveDir;
    EPlayerAttribute m_Attribute;  
    CSkillManager* m_SkillManager;
    CInterpolator* m_Interpolator;
    Stats* m_Stats;
    std::vector<std::shared_ptr<class CItem>> m_Items;

    float m_Speed = 1000.f;
};

