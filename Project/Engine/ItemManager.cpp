#include "pch.h"
#include "ItemManager.h"
#include "PathManager.h"
#include "Item.h"
#include "fstream"
#include "JSON/json.hpp"

using json = nlohmann::json;

CItemManager::CItemManager()
{
}

CItemManager::~CItemManager()
{
}

bool CItemManager::Init()
{
	auto path = CPathManager::GetInst()->FindPath(JSON_PATH);
	std::ifstream file{ (path / L"item.json") };

	if (!file.is_open())
	{
		std::cout << "파일이 없습니다." << std::endl;
		return false;
	}

	json map;

	file >> map;

	for (const auto& item : map)
	{
		std::wstring name = s2wsForJson(item["Name"]);
		std::wstring description = s2wsForJson(item["Description"]);
		std::wstring part = s2wsForJson(item["Where"]);
		std::wstring rank = s2wsForJson(item["Rank"]);
		uint32 id = std::stoi(s2wsForJson(item["Id"]));
		float amount = item["Amount"];
		uint32 price = item["Price"];

		std::shared_ptr<CItem> item = std::make_shared<CItem>(ItemInfo(id, name, description, amount, part, price, rank));
		m_ItemList[id] = item;
	}
	return true;
}
