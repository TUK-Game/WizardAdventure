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
	std::wstring animationName;
};

class CSkillData :
	public CRef
{
public:
	CSkillData(const SkillInfo& info);
	virtual ~CSkillData();

public:
	SkillInfo& GetSkillInfo() { return m_SkillInfo; }

public:
	virtual CSkillData* Clone() override { return new CSkillData(*this); }

private:
	SkillInfo m_SkillInfo;
};

