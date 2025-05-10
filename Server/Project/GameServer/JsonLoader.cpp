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
#include "MonsterTriggerBox.h"
#include "NPC.h"
#include "Item.h"
#include "ItemManager.h"
#include "Skill.h"
#include "SkillManager.h"

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

	std::unordered_map<std::string, std::pair<std::vector<Vec3>, std::vector<uint32>>> um;

	for (const auto& obj : map)
	{
		std::string name = obj["name"];
		std::vector<float> pos = obj["position"];
		std::vector<float> rot = obj["rotation"];
		std::vector<float> scale = obj["scale"];
		std::vector<float> size = obj["size"];
		std::vector<float> mat = obj["matrix"];

		Matrix matrix;
		matrix._11 = mat[0]; matrix._12 = mat[1]; matrix._13 = mat[2]; matrix._14 = mat[3];
		matrix._21 = mat[4]; matrix._22 = mat[5]; matrix._23 = mat[6]; matrix._24 = mat[7];
		matrix._31 = mat[8]; matrix._32 = mat[9]; matrix._33 = mat[10]; matrix._34 = mat[11];
		matrix._41 = mat[12]; matrix._42 = mat[13]; matrix._43 = mat[14]; matrix._44 = mat[15];

		std::vector<Vec3> vertices;
		std::vector<Vec3> worldVertices;
		std::vector<UINT32> indices;

		if (um.find(name) != um.end())
		{
			vertices = um[name].first;
			indices = um[name].second;
		}
		else
		{
			for (const auto& vertex : obj["vertex"])
			{
				vertices.push_back({ vertex[0], vertex[1], vertex[2] });
			}

			for (const auto& index : obj["index"])
			{
				indices.push_back(index);
			}
			um[name].first = vertices;
			um[name].second = indices;
		}

		for (const Vector3& localVertex : vertices)
		{
			Vec3 worldVertex = Vec3::Transform(localVertex, matrix);
			worldVertices.push_back(worldVertex);
		}

		std::vector<WorldTriangle> triangles;
		for (int i = 0; i + 2 < indices.size(); i += 3)
		{
			triangles.push_back({ worldVertices[indices[i]], worldVertices[indices[i + 1]], worldVertices[indices[i + 2]] });
		}

		if (pos[1] <= -200.f)
			continue;

		CGameObjectRef object = CObjectUtil::CreateObject();
		object->m_Triangles = triangles;
		object->GetCollider()->SetBoxInfo(Vec3(pos[0], pos[1], pos[2]), Vec3(size[0] * scale[0], size[1] * scale[1], size[2] * scale[2]), Vec3(rot[0], rot[1], rot[2]));
		object->GetCollider()->SetWorldTriangle(triangles);
		object->GetCollider()->SetCollisionProfile("Wall");
		room->GetLevelCollision()->AddCollider(object->GetCollider(), ECollision_Channel::Wall);
		room->AddObject((uint32)EObject_Type::Wall, object);
	}
	LoadMonster(fileName, room);
	LoadNPC(fileName, room);
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

	json map;

	file >> map;

	const auto& mo = room->GetLayerObjects((uint32)(EObject_Type::TRIGGER));

	for (const auto& obj : map)
	{
		std::string name = obj["name"];
		std::vector<float> pos = obj["position"];
		std::vector<float> rot = obj["rotation"];
		std::vector<float> scale = obj["scale"];
		std::vector<float> size = obj["size"];

		CMonsterRef object = CObjectUtil::CreateMonster();
		object->GetCollider()->SetBoxInfo(Vec3(pos[0], pos[1], pos[2]), Vec3(size[0] * scale[0], size[1] * scale[1], size[2] * scale[2]),
			Vec3(rot[0], rot[1], rot[2]), Vec3(0, size[1] * scale[1] / 2, 0));

		object->GetCollider()->SetCollisionProfile("Monster");
		object->MonsterInfo->mutable_object_info()->mutable_pos_info()->set_state(Protocol::MOVE_STATE_IDLE);
		object->MonsterInfo->mutable_object_info()->mutable_pos_info()->mutable_position()->set_x(pos[0]); 
		object->MonsterInfo->mutable_object_info()->mutable_pos_info()->mutable_position()->set_y(pos[1]); 
		object->MonsterInfo->mutable_object_info()->mutable_pos_info()->mutable_position()->set_z(pos[2]); 
		object->MonsterInfo->mutable_object_info()->mutable_pos_info()->mutable_size()->set_x(scale[0]); 
		object->MonsterInfo->mutable_object_info()->mutable_pos_info()->mutable_size()->set_y(scale[1]); 
		object->MonsterInfo->mutable_object_info()->mutable_pos_info()->mutable_size()->set_z(scale[2]); 
		object->MonsterInfo->mutable_object_info()->mutable_pos_info()->mutable_rotation()->set_x(rot[0]); 
		object->MonsterInfo->mutable_object_info()->mutable_pos_info()->mutable_rotation()->set_y(rot[1]); 
		object->MonsterInfo->mutable_object_info()->mutable_pos_info()->mutable_rotation()->set_z(rot[2]); 

		object->SetState(Protocol::MOVE_STATE_SPAWN);	// spawn
		room->AddMonster(object);

		for (const auto& box : mo)
		{
			CMonseterTriggerBoxRef trigger = dynamic_pointer_cast<CMonseterTriggerBox>(box.second);
			if (!trigger)
				continue;

			if (trigger->IsMonsterInArea(Vec3(pos[0], pos[1], pos[2])))
			{
				trigger->AddMonster(object);
				break;
			}
		}
		//return;
		//room->AddObject((uint32)EObject_Type::Monster, object);
		//break;
	}


	std::cout << "Json read완료" << std::endl;
}

#define MAX_ITEMS_NUMBER 10012
#define MAX_SKILLS_NUMBER 30003
void CJsonLoader::LoadNPC(const std::wstring& fileName, CRoomRef room)
{
	constexpr int maxItem = 8;
	constexpr int maxSkill = 3;

	std::random_device rd;
	std::default_random_engine dre(rd());
	std::uniform_int_distribution randomItem{10001, MAX_ITEMS_NUMBER };
	std::uniform_int_distribution randomSkill{30001, MAX_SKILLS_NUMBER };
	std::wstring path = L"..\\..\\..\\Content\\Json\\" + fileName + L"_NPC.json";

	std::ifstream file{ path.c_str() };

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
		std::vector<float> rot = obj["rotation"];
		std::vector<float> scale = obj["scale"];
		std::vector<float> size = obj["size"];

		CNPCRef object = CObjectUtil::CreateObject<CNPC>();
		object->GetCollider()->SetBoxInfo(Vec3(pos[0], pos[1], pos[2]), Vec3(size[0], size[1], size[2]), Vec3(rot[0], rot[1], rot[2]), Vec3(0, 100, 0));
		object->GetCollider()->SetCollisionProfile("NPC");
		object->ObjectInfo->mutable_pos_info()->set_state(Protocol::MOVE_STATE_IDLE);

		auto& itemList = object->GetItemList();
		while (itemList.size() < maxItem)
		{
			uint32 itemId = randomItem(dre);
			auto iter = std::find_if(itemList.begin(), itemList.end(), [&](const CItemRef item) {
				return itemId == item->GetItemInfo().id;
				});

			if (iter != itemList.end())
				continue;

			const auto& item = g_ItemManager->FindItem(itemId);
			itemList.emplace_back(item);
		}	

		auto& skillList = object->GetSkillList();
		while (skillList.size() < maxSkill)
		{
			uint32 skillId = randomSkill(dre);
			auto iter = std::find_if(skillList.begin(), skillList.end(), [&](const CSkillRef item) {
				return skillId == item->GetSkillInfo().id;
				});

			if (iter != skillList.end())
				continue;

			const auto& skill = g_SkillManager->FindSkill(skillId);
			skillList.emplace_back(skill);
		}

		room->GetLevelCollision()->AddCollider(object->GetCollider(), ECollision_Channel::NPC);
		room->AddObject((uint32)EObject_Type::NPC, object);
	}
}

void CJsonLoader::LoadItem(const std::wstring& fileName, std::unordered_map<uint32, CItemRef>& itemMap)
{
	std::wstring path = L"..\\..\\..\\Content\\Json\\" + fileName + L".json";

	std::ifstream file{ path.c_str() };

	if (!file.is_open())
	{
		std::cout << "파일이 없습니다." << std::endl;
		return;
	}

	json map;

	file >> map;

	for (const auto& item : map)
	{
		std::wstring name = s2ws(item["Name"]);
		std::wstring description = s2ws(item["Description"]);
		std::wstring part = s2ws(item["Where"]);
		std::wstring rank = s2ws(item["Rank"]);
		uint32 id = std::stoi(s2ws(item["Id"]));
		float amount = item["Amount"];
		uint32 price = item["Price"];

		EITEM_PART itemPart = CItemManager::ConvertToItemPart(part);
		CItemRef item = std::make_shared<CItem>(ItemInfo(id, name, description, amount, itemPart, price, rank));
		itemMap[id] = item;
	}
}

void CJsonLoader::LoadSkill(const std::wstring& fileName, std::unordered_map<uint32, CSkillRef>& skillMap)
{
	std::wstring path = L"..\\..\\..\\Content\\Json\\" + fileName + L".json";

	std::ifstream file{ path.c_str() };

	if (!file.is_open())
	{
		std::cout << "파일이 없습니다." << std::endl;
		return;
	}

	json map;

	file >> map;

	for (const auto& s : map)
	{
		std::wstring name = s2ws(s["Name"]);
		std::wstring description = s2ws(s["Description"]);
		std::wstring attribute = s2ws(s["Attribute"]);
		std::wstring keytype = s2ws(s["Type"]);
		std::wstring animationName = s2ws(s["AnimationName"]);
		uint32 id = std::stoi(s2ws(s["Id"]));
		uint32 price = s["Price"];
		float ratio = s["DamageRatio"];
		float cooltime = s["CoolTime"];
		float explosionime = s["ExplosionTime"];

		CSkillRef skill = std::make_shared<CSkill>(SkillInfo(id, name, description, ratio, attribute, price, keytype, cooltime));
		skillMap[id] = skill;
	}
}
