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
		Vec3 finalScale = {
		size[0] * scale[0],
		size[1] * scale[1],
		size[2] * scale[2]
		};

		Matrix scaleMat = XMMatrixScaling(scale[0], scale[1], scale[2]);
		XMVECTOR qRot = XMQuaternionRotationRollPitchYaw(rot[0], rot[1], rot[2]);
		Matrix rotMat = XMMatrixRotationQuaternion(qRot);

		Matrix transMat = XMMatrixTranslation(pos[0], pos[1], pos[2]);

		Matrix worldMat = scaleMat * rotMat * transMat;

		for (const Vector3& localVertex : vertices)
		{
			Vec3 worldVertex = Vec3::Transform(localVertex, worldMat);
			worldVertices.push_back(worldVertex);
		}

		std::vector<WorldTriangle> triangles;
		for (int i = 0; i + 2 < indices.size(); i += 3)
		{
			triangles.push_back({ worldVertices[indices[i]], worldVertices[indices[i + 1]], worldVertices[indices[i + 2]] });
		}

		CGameObjectRef object = CObjectUtil::CreateObject();
		object->m_Triangles = triangles;
		object->GetCollider()->SetBoxInfo(XMFLOAT3(pos[0], pos[1], pos[2]), XMFLOAT3(size[0] * scale[0], size[1] * scale[1], size[2] * scale[2]), XMFLOAT3(rot[0], rot[1], rot[2]));
		object->GetCollider()->SetWorldTriangle(triangles);
		object->GetCollider()->SetCollisionProfile("Wall");
		room->GetLevelCollision()->AddCollider(object->GetCollider(), ECollision_Channel::Wall);
		room->AddObject((uint32)EObject_Type::Wall, object);
	}
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

	json map;

	file >> map;


	for (const auto& obj : map)
	{
		std::string name = obj["name"];
		std::vector<float> pos = obj["position"];
		std::vector<float> rot = obj["rotation"];
		std::vector<float> scale = obj["scale"];
		std::vector<float> size = obj["size"];

		CMonsterRef object = CObjectUtil::CreateMonster();
		object->GetCollider()->SetBoxInfo(XMFLOAT3(pos[0], pos[1], pos[2]), XMFLOAT3(size[0], size[1], size[2]), XMFLOAT3(rot[0], rot[1], rot[2]), XMFLOAT3(0, 100, 0));
		object->GetCollider()->SetCollisionProfile("Monster");
		object->MonsterInfo->mutable_object_info()->mutable_pos_info()->set_state(Protocol::MOVE_STATE_IDLE);
		object->SetState(Protocol::MOVE_STATE_IDLE);
		room->AddObject((uint32)EObject_Type::Monster, object);
		break;
	}


	std::cout << "Json read완료" << std::endl;
}

void CJsonLoader::Load(std::ifstream& file, CRoomRef room, ECollision_Channel channel)
{
	
}
