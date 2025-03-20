#include "pch.h"
#include "SkillManager.h"

CSkillManager::CSkillManager(EPlayerAttribute attribute)
    : m_Attribute(attribute) {}

void CSkillManager::UseSkill(int skillIndex)
{
    switch (m_Attribute)
    {
    case EPlayerAttribute::Fire:
        if (skillIndex == 0) std::cout << "Fireball (Q)!" << std::endl;
        if (skillIndex == 1) std::cout << "Flame Wave (W)!" << std::endl;
        if (skillIndex == 2) std::cout << "Meteor Strike (E)!" << std::endl;
        break;

    case EPlayerAttribute::Water:
        if (skillIndex == 0) std::cout << "Water Jet (Q)!" << std::endl;
        if (skillIndex == 1) std::cout << "Ice Shield (W)!" << std::endl;
        if (skillIndex == 2) std::cout << "Tsunami (E)!" << std::endl;
        break;

    case EPlayerAttribute::Electric:
        if (skillIndex == 0) std::cout << "Thunderbolt (Q)!" << std::endl;
        if (skillIndex == 1) std::cout << "Lightning Shield (W)!" << std::endl;
        if (skillIndex == 2) std::cout << "Storm Burst (E)!" << std::endl;
        break;
    }
}