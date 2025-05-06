#pragma once
#include "GameObject.h"
#include "SkillInfo.h"


class CSkillManager
{
public:
    CSkillManager(EPlayerAttribute attribute, CGameObject* owner);
    void UseSkill(int skillIndex, float duration = 0.f);
    void LearnSkill(int slotIndex, ESkillType skill);
    void ForgetSkill(int slotIndex);
    ESkillType GetEquippedSkill(int slotIndex) const;
    void UpdateCooldowns(float deltaTime);
    const std::array<float, 5>& GetSkillCooldowns() const { return m_SkillCooldowns; }
    float GetSkillCooldown(int slotIndex) const;
    ESkillSlot GetNextSlot() { return m_NextSlot; }

    void SetNextSlot() { m_NextSlot = static_cast<ESkillSlot>(static_cast<int>(m_NextSlot) + 1); }

    ESkillType ConvertSkillNameToType(const std::wstring& name);
private:
    void CastFireballTowardMouse();
    void CastFireballTowardQ(float duration);
    void CastMeteor();
    void SpawnFireTowerAtMouse(float duration);
    void FireSwordSpreadShot(float duration);


private:
    Vec3 CalculateMouseDirectionFromPlayerTopView(const Vec3& fromPos);
    Vec3 CalculateMouseDirectionFromPos(const Vec3& fromPos);
    Vec3 GetMouseGroundPoint();
    void SetLookRotationY(const Vec3& dir);

private:
    std::array<ESkillType, 5> m_SkillSlots = { ESkillType::None, ESkillType::None, ESkillType::None, ESkillType::None, ESkillType::None };
    std::array<float, 5> m_SkillCooldowns = { 0.f, 0.f, 0.f, 0.f, 0.f }; 
    EPlayerAttribute m_Attribute;
    CGameObject* m_Owner;
    ESkillSlot m_NextSlot = ESkillSlot::Q;
};

