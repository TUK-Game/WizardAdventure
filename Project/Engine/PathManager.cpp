#include "pch.h"
#include "PathManager.h"

CPathManager::CPathManager()
{
}

CPathManager::~CPathManager()
{
}

void CPathManager::Init()
{
	TCHAR root[MAX_PATH] = {};
	GetModuleFileName(0, root, MAX_PATH);	// 실행파일 경로

	std::filesystem::path path(root);
	path = path.parent_path();	//Client
	path = path.parent_path();	//Bin
	path = path.parent_path();	//Solution Directory
	path /= "Content";		// '/'operator : 플랫폼에 따라 '/' 또는 '\\' 결합

	m_mapPath.insert(std::make_pair(ROOT_PATH, path));

	AddPath(HLSL_PATH, TEXT("Shaders"));
	AddPath(TEXTURE_PATH, TEXT("Texture"));
	AddPath(SOUND_PATH, TEXT("Sound"));
	//AddPath(FONT_PATH, TEXT("Font"));
}

bool CPathManager::AddPath(const std::wstring& name, const std::wstring& directoryName, const std::wstring& basePathName)
{
	if (!FindPath(name).empty())
		return false;

	std::filesystem::path path = FindPath(basePathName);
	path /= directoryName;
	path /= "";

	m_mapPath.insert(std::make_pair(name, path));

	return true;
}

std::filesystem::path CPathManager::FindPath(const std::wstring& name)
{
	auto iter = m_mapPath.find(name);
	if (iter == m_mapPath.end())
		return "";

	return iter->second;
}
