#include "pch.h"
#include "JsonLoader.h"
#include "json.hpp"
#include "Room.h"
#include "GameObject.h"	
#include <iostream>
#include <fstream>

using json = nlohmann::json;

void CJsonLoader::LoadMap(const std::wstring& fileName, CRoomRef room)
{
	std::wstring path = L"..\\..\\..\\Content\\Json\\" + fileName + L".json";

	std::ifstream file{ path.c_str()};
	
	if (!file.is_open())
	{
		std::cout << "파일이 없습니다." << std::endl;
		return;
	}

	json map;

	file >> map;

	for (const auto& obj : map)
	{
		std::string name = obj["name"];
		std::vector<float> pos = obj["position"];
		
		CGameObject* object = new CGameObject;

		//room->AddObject();
	}

}
