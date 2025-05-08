#pragma once

enum class EITEM_PART
{
	ATTACK,
	MAXHP,
	HEAL,
	SPEED,

	END,
};

struct ItemInfo
{
	uint32 id;
	std::wstring name;
	std::wstring description;
	float amount;
	EITEM_PART part;
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

