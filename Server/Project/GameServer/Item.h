#pragma once

struct ItemInfo
{
	uint32 id;
	std::wstring name;
	std::wstring description;
	float amount;
	std::wstring part;
	int price;
	std::wstring rank;
	bool bSell = false;
};

class CItem
{
	friend class CItemManager;
public:
	CItem(const ItemInfo& info);
	~CItem();

public:
	ItemInfo& GetItemInfo() { return m_ItemInfo; }

private:
	ItemInfo m_ItemInfo;
};

