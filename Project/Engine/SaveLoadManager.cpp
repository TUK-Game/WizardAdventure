#include "pch.h"
#include "SaveLoadManager.h"
#include "Engine.h"
#include "PathManager.h"
#include "LevelManager.h"
#include "Level.h"
#include "Layer.h"
#include "GameObject.h"
#include "fstream"
#include "Camera.h"
#include "Transform.h"
#include "BaseCollider.h"
#include "Light.h"

CSaveLoadManager::CSaveLoadManager()
{
}

CSaveLoadManager::~CSaveLoadManager()
{
}

void CSaveLoadManager::Save()
{
	CLevel* level = CLevelManager::GetInst()->GetCurrentLevel();
	auto path = CPathManager::GetInst()->FindPath(MAP_PATH);
	std::wstring filename = level->GetName() + std::wstring(L".bin");
	std::ofstream file(path / filename, std::ios::binary | std::ios::out | std::ios::trunc);

	if (!file.is_open())
	{
		std::cerr << "파일을 열 수 없습니다: " << path / filename << std::endl;
		return;
	}

	std::cout << "저장완료" << std::endl;
	file.close();
}

void CSaveLoadManager::Load(const std::wstring& filename)
{
	auto path = CPathManager::GetInst()->FindPath(MAP_PATH);
	std::ifstream file(path / (filename + std::wstring(L".bin")), std::ios::binary);
	if (!file.is_open())
	{
		std::cerr << "파일을 열 수 없습니다: " << path / filename << std::endl;
		return;
	}


	std::cout << "로드완료" << std::endl;
	file.close();
}

void CSaveLoadManager::WriteString(const std::wstring& str, std::ofstream& file)
{
	if (str.empty())
		return;

	std::string s = ws2s(str);
	WriteString(s, file);
}

void CSaveLoadManager::WriteString(std::string& str, std::ofstream& file)
{
	if (str.empty()) 
		return;

	uint32_t  len = static_cast<uint32_t>(str.size());
	file.write(reinterpret_cast<const char*>(&len), sizeof(len));
	file.write(str.c_str(), len);
}

void CSaveLoadManager::WriteString(const char* str, std::ofstream& file)
{
	std::string s = str;
	WriteString(s, file);
}
