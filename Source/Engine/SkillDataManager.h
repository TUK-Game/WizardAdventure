#pragma once
class CSkillDataManager :
	public CSingleton<CSkillDataManager>

{
	DECLARE_SINGLETON(CSkillDataManager)

public:
	bool Init();
	std::shared_ptr<class CSkillData> FindSkill(uint32 id) { return m_SkillList[id]; }
	std::unordered_map<uint32, std::shared_ptr<class CSkillData>>& GetSkillList() { return m_SkillList; }
private:
	std::unordered_map<uint32, std::shared_ptr<class CSkillData>> m_SkillList;
};

