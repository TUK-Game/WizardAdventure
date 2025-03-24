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
        if (skillIndex == 1) std::cout << "Flame Wave (E)!" << std::endl;
        if (skillIndex == 2) std::cout << "Meteor Strike (R)!" << std::endl;
        if (skillIndex == 3) std::cout << "(LButton)!" << std::endl;
        break;

    case EPlayerAttribute::Water:
        if (skillIndex == 0) std::cout << "Water Jet (Q)!" << std::endl;
        if (skillIndex == 1) std::cout << "Ice Shield (E)!" << std::endl;
        if (skillIndex == 2) std::cout << "Tsunami (R)!" << std::endl;
        if (skillIndex == 3) std::cout << "(LButton)!" << std::endl;
        break;

    case EPlayerAttribute::Electric:
        if (skillIndex == 0) std::cout << "Thunderbolt (Q)!" << std::endl;
        if (skillIndex == 1) std::cout << "Lightning Shield (E)!" << std::endl;
        if (skillIndex == 2) std::cout << "Storm Burst (R)!" << std::endl;
        if (skillIndex == 3) std::cout << "(LButton)!" << std::endl;
        break;
    }
}