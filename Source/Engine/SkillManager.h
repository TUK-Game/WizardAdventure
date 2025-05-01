#pragma once
#include "GameObject.h"

class CSkillManager
{
public:
    CSkillManager(EPlayerAttribute attribute, CGameObject* owner);
    void UseSkill(int skillIndex, float duration = 0.f);

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
    EPlayerAttribute m_Attribute;
    CGameObject* m_Owner;
};

