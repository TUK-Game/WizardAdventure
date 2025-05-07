#pragma once
class CSkillManager
{
public:
	CSkillManager();
	~CSkillManager();

public:
	void Init();
	CSkillRef FindSkill(uint32 id) { return m_SkillMap[id]; }


private:
	std::unordered_map<uint32, CSkillRef> m_SkillMap;
};

extern CSkillManagerRef g_SkillManager;