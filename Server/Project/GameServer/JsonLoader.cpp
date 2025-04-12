#include "pch.h"
#include "JsonLoader.h"
#include "json.hpp"
#include "Room.h"
#include "GameObject.h"	
#include "BoxCollider.h"
#include "CollisionManager.h"
#include "ObjectUtil.h"
#include "LevelCollision.h"
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
		std::vector<float> size = obj["size"];
		
		CGameObjectRef object = CObjectUtil::CreateObject();
		object->GetCollider()->SetCollisionProfile("Wall");
		object->GetCollider()->SetBoxInfo(XMFLOAT3(pos[0], pos[1], pos[2]), XMFLOAT3(size[0], size[1], size[2]));
		room->GetLevelCollision()->AddCollider(object->GetCollider(), ECollision_Channel::Wall);
		room->AddObject(object);
	}
	std::cout << "Json read완료" << std::endl;
}
