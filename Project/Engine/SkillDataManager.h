#pragma once
class CSkillDataManager :
	public CSingleton<CSkillDataManager>

{
	DECLARE_SINGLETON(CSkillDataManager)

public:
	bool Init();
	std::shared_ptr<class CSkillData> FindSkill(uint32 id) { return m_SkillList[id]; }
	uint32 FindSkillId(const std::wstring& skillName) { return m_SkillIdMap[skillName]; }
	std::unordered_map<uint32, std::shared_ptr<class CSkillData>>& GetSkillList() { return m_SkillList; }
	std::unordered_map<std::wstring, uint32>& GetSkillIdMap() { return m_SkillIdMap; }

private:
	std::unordered_map<uint32, std::shared_ptr<class CSkillData>> m_SkillList;
	std::unordered_map<std::wstring, uint32> m_SkillIdMap;
};

