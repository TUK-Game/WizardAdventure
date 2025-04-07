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
    void CastMeteor();
    void SpawnFirePillarAtMouse();
    void FireSwordSpreadShot();


private:
    Vec3 CalculateMouseDirectionFromPlayerTopView(const Vec3& fromPos);
    Vec3 CalculateMouseDirectionFromPos(const Vec3& fromPos);
    Vec3 GetMouseGroundPoint();
    void SetLookRotationY(const Vec3& dir);

private:
    EPlayerAttribute m_Attribute;
    CGameObject* m_Owner;
};

