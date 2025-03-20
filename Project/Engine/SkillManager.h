#pragma once
class CSkillManager
{
public:
    CSkillManager(EPlayerAttribute attribute);
    void UseSkill(int skillIndex);

private:
    EPlayerAttribute m_Attribute;
};

