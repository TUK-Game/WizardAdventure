#include "pch.h"
#include "ItemManager.h"
#include "JsonLoader.h"
CItemManagerRef g_ItemManager = std::make_shared<CItemManager>();

CItemManager::CItemManager()
{
}

CItemManager::~CItemManager()
{
}

void CItemManager::Init()
{
	CJsonLoader::LoadItem(L"item", m_ItemMap);
}
