#include "pch.h"
#include "SkillManager.h"
#include "JsonLoader.h"

CSkillManagerRef g_SkillManager = std::make_shared<CSkillManager>();

CSkillManager::CSkillManager()
{
}

CSkillManager::~CSkillManager()
{
}

void CSkillManager::Init()
{
	CJsonLoader::LoadSkill(L"skill", m_SkillMap);
}
