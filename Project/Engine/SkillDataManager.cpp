#include "pch.h"
#include "SkillDataManager.h"
#include "PathManager.h"
#include "SkillData.h"
#include "fstream"
#include "JSON/json.hpp"

using json = nlohmann::json;

CSkillDataManager::CSkillDataManager()
{
}

CSkillDataManager::~CSkillDataManager()
{
}

bool CSkillDataManager::Init()
{
	auto path = CPathManager::GetInst()->FindPath(JSON_PATH);
	std::ifstream file{ (path / L"skill.json") };

	if (!file.is_open())
	{
		std::cout << "파일이 없습니다." << std::endl;
		return false;
	}

	json map;

	file >> map;

	for (const auto& s : map)
	{
		std::wstring name = s2wsForJson(s["Name"]);
		std::wstring description = s2wsForJson(s["Description"]);
		std::wstring attribute = s2wsForJson(s["Attribute"]);
		std::wstring keytype = s2wsForJson(s["Type"]);
		std::wstring animationName = s2wsForJson(s["AnimationName"]);
		uint32 id = std::stoi(s2wsForJson(s["Id"]));
		uint32 price = s["Price"];
		float ratio = s["DamageRatio"];
		float cooltime = s["CoolTime"];

		std::shared_ptr<CSkillData> skill = std::make_shared<CSkillData>(SkillInfo(id, name, description, ratio, attribute, price, keytype, cooltime, animationName));
		m_SkillList[id] = skill;
	}
	return true;
}
