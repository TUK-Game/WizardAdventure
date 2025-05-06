#pragma once

struct SkillInfo
{
	uint32 id;
	std::wstring name;
	std::wstring description;
	float damageRatio;
	std::wstring attribute;
	int price;
	std::wstring keyType;
	float cooltime;
	bool bSell = false;
};

class CSkill
{
public:
	CSkill(const SkillInfo& info);
	~CSkill();

public:
	SkillInfo& GetSkillInfo() { return m_SkillInfo; }

private:
	SkillInfo m_SkillInfo;
};

