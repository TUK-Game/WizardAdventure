#pragma once

namespace Skill
{
    struct SkillInfo
    {
        float damage;
        float cooldown;
    };

    constexpr SkillInfo FireBall = { 0.7f, 0.f };
    constexpr SkillInfo FireBallQ = { 2.0f, 3.0f };
    constexpr SkillInfo Meteor = { 2.5f, 0.f };
    constexpr SkillInfo FireSword = { 0.9f, 8.0f };
    constexpr SkillInfo FireTower = { 0.8f, 10.0f };
}

enum ESkillSlot
{
    LButton,
    RButton,
    Q,
    E,
    R,
};