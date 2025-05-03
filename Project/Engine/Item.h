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
};

class CItem :
    public CRef
{
public:
	CItem(const ItemInfo& info);
	virtual ~CItem();

public:
	ItemInfo& GetItemInfo() { return m_ItemInfo; }

public:
	virtual CItem* Clone() override { return new CItem(*this); }

private:
	ItemInfo m_ItemInfo;
};

