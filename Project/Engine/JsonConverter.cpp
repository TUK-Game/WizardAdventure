#include "pch.h"
#include "JsonConverter.h"
#include "JSON/json.hpp"
#include "LevelManager.h"
#include "Level.h"
#include "Transform.h"
#include "GameObject.h"
#include "SubLevel.h"
#include "fstream"
#include "MeshRenderer.h"
#include "PathManager.h"
#include "BaseCollider.h"
#include "Layer.h"

using json = nlohmann::json;

CJsonConverter::CJsonConverter()
{
}

CJsonConverter::~CJsonConverter()
{
}

void CJsonConverter::SaveMapCollision(const std::wstring& fileName)
{
    auto path = CPathManager::GetInst()->FindPath(JSON_PATH);
	CLevel* level = CLevelManager::GetInst()->GetCurrentLevel();
	std::vector<CGameObject*> objects;
	level->m_SubLevel->PickGameObject(objects);

    std::ofstream file{ path / (fileName + L".json") };

    json map = json::array();
    std::unordered_map<std::string, bool> um;
    for (auto object : objects)
    {
        json obj;
        CTransform* transform = object->GetTransform();
        CBaseCollider* collider = object->GetCollider();
        Vec3 pos = transform->GetRelativePosition();
        Vec3 rot = transform->GetRelativeRotation();
        Vec3 scale = transform->GetRelativeScale();
        Matrix mat = transform->GetWorldMatrix();
        auto mesh = object->GetMeshRenderer()->GetMesh();
        std::string meshName = ws2s(mesh->GetName());
        const std::vector<Vertex>& vertices = mesh->m_vertexInfo;
        const std::vector<UINT32>& indicse = mesh->m_indexInfo;
        meshName.erase(std::find(meshName.begin(), meshName.end(), '\0'), meshName.end());

        obj["name"] = meshName;
        obj["position"] = { pos.x, pos.y, pos.z };
        obj["rotation"] = { rot.x, rot.y, rot.z };
        obj["scale"] = { scale.x, scale.y, scale.z };
        obj["size"] = { collider->size.x, collider->size.y, collider->size.z };
        obj["matrix"] = { mat._11, mat._12, mat._13, mat._14,
                          mat._21, mat._22, mat._23, mat._24,
                          mat._31, mat._32, mat._33, mat._34,
                          mat._41, mat._42, mat._43, mat._44 };

        if (um[meshName])
        {
            map.push_back(obj);
            continue;
        }

        um[meshName] = true;
        for (const Vertex& v : vertices)
        {
            obj["vertex"].push_back({ v.Pos.x, v.Pos.y, v.Pos.z });
        }
        obj["index"] = indicse;

        map.push_back(obj);
    }

    file << map.dump(4);

    SaveMonster(fileName);
}

void CJsonConverter::SaveMonster(const std::wstring& fileName)
{
    auto path = CPathManager::GetInst()->FindPath(JSON_PATH);
    CLevel* level = CLevelManager::GetInst()->GetCurrentLevel();
    
    std::vector<CGameObject*> objects = level->GetLayer(11)->GetParentObjects(); // monster layer

    std::ofstream file{ path / (fileName + L"_Monster.json") };

    json map = json::array();
    std::unordered_map<std::string, bool> um;
    for (auto object : objects)
    {
        json obj;
        CTransform* transform = object->GetTransform();
        CBaseCollider* collider = object->GetCollider();
        Vec3 pos = transform->GetRelativePosition();
        Vec3 rot = transform->GetRelativeRotation();
        Vec3 scale = transform->GetRelativeScale();
        std::string name = ws2s(object->GetName());
        name.erase(std::find(name.begin(), name.end(), '\0'), name.end());

        obj["name"] = name;
        obj["position"] = { pos.x, pos.y, pos.z };
        obj["rotation"] = { rot.x, rot.y, rot.z };
        obj["scale"] = { scale.x, scale.y, scale.z };
        obj["size"] = { collider->size.x, collider->size.y, collider->size.z };

        if (um[name])
        {
            map.push_back(obj);
            continue;
        }

        um[name] = true;
        map.push_back(obj);
    }

    file << map.dump(4);

    std::cout << "JSON작성완료" << std::endl;
}

void CJsonConverter::Save(std::ofstream& file, const std::vector<CGameObject*>& objects)
{
   
}
