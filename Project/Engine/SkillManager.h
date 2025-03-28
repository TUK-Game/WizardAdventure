#pragma once
#include "GameObject.h"

class CSkillManager
{
public:
    CSkillManager(EPlayerAttribute attribute, CGameObject* owner);
    void UseSkill(int skillIndex);

private:
    void CastFireballTowardMouse();
    void CastFireballTowardQ();
    void SpawnFirePillarAtMouse();

private:
    Vec3 CalculateMouseDirectionFromPlayerTopView(const Vec3& fromPos);
    Vec3 GetMouseGroundPoint();
private:
    EPlayerAttribute m_Attribute;
    CGameObject* m_Owner;
};

