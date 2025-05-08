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

EITEM_PART CItemManager::ConvertToItemPart(const std::wstring& part)
{
	if (part == L"ATK")
		return EITEM_PART::ATTACK;
	if (part == L"HP")
		return EITEM_PART::MAXHP;
	if (part == L"SPEED")
		return EITEM_PART::SPEED;
	if (part == L"HEAL")
		return EITEM_PART::HEAL;

	return EITEM_PART::END;
}
