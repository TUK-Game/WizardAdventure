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

    for (auto object : objects)
    {
        json obj;
        CTransform* transform = object->GetTransform();
        CBaseCollider* collider = object->GetCollider();
        Vec3 pos = transform->GetWorldPosition();
        std::string name = ws2s(object->GetMeshRenderer()->GetMesh()->GetName());
        name.erase(std::find(name.begin(), name.end(), '\0'), name.end());
        obj["name"] = name;
        obj["position"] = { pos.x, pos.y, pos.z };
        obj["size"] = { collider->size.x, collider->size.y, collider->size.z };

        map.push_back(obj);
    }

    file << map.dump(4);
    std::cout << "JSON작성완료" << std::endl;
}
