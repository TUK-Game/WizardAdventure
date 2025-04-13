#include "pch.h"
#include "JsonLoader.h"
#include "json.hpp"
#include "Room.h"
#include "GameObject.h"	
#include "BoxCollider.h"
#include "CollisionManager.h"
#include "ObjectUtil.h"
#include "LevelCollision.h"
#include "Monster.h"
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

	Load(file, room, ECollision_Channel::Wall);
	LoadMonster(fileName, room);
}

void CJsonLoader::LoadMonster(const std::wstring& fileName, CRoomRef room)
{
	std::wstring path = L"..\\..\\..\\Content\\Json\\" + fileName + L"_Monster.json";

	std::ifstream file{ path.c_str() };

	if (!file.is_open())
	{
		std::cout << "파일이 없습니다." << std::endl;
		return;
	}

	Load(file, room, ECollision_Channel::Monster);
	std::cout << "Json read완료" << std::endl;
}

void CJsonLoader::Load(std::ifstream& file, CRoomRef room, ECollision_Channel channel)
{
	json map;

	file >> map;

	for (const auto& obj : map)
	{
		std::string name = obj["name"];
		std::vector<float> pos = obj["position"];
		std::vector<float> size = obj["size"];

		switch (channel)
		{
		case ECollision_Channel::Monster:
		{
			CMonsterRef object = CObjectUtil::CreateMonster();
			object->GetCollider()->SetBoxInfo(XMFLOAT3(pos[0], pos[1], pos[2]), XMFLOAT3(size[0], size[1], size[2]), XMFLOAT3(0, 100, 0));
			object->GetCollider()->SetCollisionProfile("Monster");
			object->MonsterInfo->mutable_object_info()->mutable_pos_info()->set_state(Protocol::MOVE_STATE_IDLE);
			object->SetState(Protocol::MOVE_STATE_IDLE);
			room->AddObject((uint32)EObject_Type::Monster, object);
			break;
		}
		case ECollision_Channel::Wall:
		{
			CGameObjectRef object = CObjectUtil::CreateObject();
			object->GetCollider()->SetBoxInfo(XMFLOAT3(pos[0], pos[1], pos[2]), XMFLOAT3(size[0], size[1], size[2]));
			object->GetCollider()->SetCollisionProfile("Wall");
			room->GetLevelCollision()->AddCollider(object->GetCollider(), channel);
			room->AddObject((uint32)EObject_Type::Wall, object);
			break;
		}
		}
	}
}
