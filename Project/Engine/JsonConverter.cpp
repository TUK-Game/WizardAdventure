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

    Save(file, objects);

    SaveMonster(fileName);
}

void CJsonConverter::SaveMonster(const std::wstring& fileName)
{
    auto path = CPathManager::GetInst()->FindPath(JSON_PATH);
    CLevel* level = CLevelManager::GetInst()->GetCurrentLevel();
    
    std::vector<CGameObject*> objects = level->GetLayer(11)->GetParentObjects(); // monster layer

    std::ofstream file{ path / (fileName + L"_Monster.json") };

    Save(file, objects);

    std::cout << "JSON작성완료" << std::endl;
}

void CJsonConverter::Save(std::ofstream& file, const std::vector<CGameObject*>& objects)
{
    json map = json::array();

    for (auto object : objects)
    {
        json obj;
        CTransform* transform = object->GetTransform();
        CBaseCollider* collider = object->GetCollider();
        Vec3 pos = transform->GetWorldPosition();
        std::string name = ws2s(object->GetName());
        name.erase(std::find(name.begin(), name.end(), '\0'), name.end());
        obj["name"] = name;
        obj["position"] = { pos.x, pos.y, pos.z };
        obj["size"] = { collider->size.x, collider->size.y, collider->size.z };

        map.push_back(obj);
    }

    file << map.dump(4);
}
