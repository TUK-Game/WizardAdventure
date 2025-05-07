#pragma once
#include "GameObject.h"
#include "SkillInfo.h"

class CSkillData;
class CSkillManager
{
public:
	CSkillManager(EPlayerAttribute attribute, CGameObject* owner);
	void UseSkill(int skillIndex, float duration = 0.f);
	void LearnSkill(int slotIndex, std::shared_ptr<CSkillData> skill);
	void ForgetSkill(int slotIndex);
	ESkillType GetEquippedSkill(int slotIndex) const;
	void UpdateCooldowns(float deltaTime);
	const std::array<float, 5>& GetSkillCooldowns() const { return m_SkillCooldowns; }
	float GetSkillCooldown(int slotIndex) const;
	ESkillSlot GetNextSlot() { return m_NextSlot; }

	void SetNextSlot() { m_NextSlot = static_cast<ESkillSlot>(static_cast<int>(m_NextSlot) + 1); }
private:
	void CastFireballTowardMouse(float damageRatio);
	void CastFireballTowardQ(float damageRatio, float duration);
	void CastMeteor(float damageRatio);
	void SpawnFireTowerAtMouse(float damageRatio, float duration);
	void FireSwordSpreadShot(float damageRatio, float duration);


private:
	Vec3 CalculateMouseDirectionFromPlayerTopView(const Vec3& fromPos);
	Vec3 CalculateMouseDirectionFromPos(const Vec3& fromPos);
	Vec3 GetMouseGroundPoint();
	void SetLookRotationY(const Vec3& dir);

private:
	std::array<std::shared_ptr<CSkillData>, 5> m_SkillSlots{ nullptr,nullptr,nullptr,nullptr,nullptr };
	std::array<float, 5> m_SkillCooldowns = { 0.f, 0.f, 0.f, 0.f, 0.f };
	EPlayerAttribute m_Attribute;
	CGameObject* m_Owner;
	ESkillSlot m_NextSlot = ESkillSlot::Q;
};

