#pragma once


class CItemManager
{
public:
	CItemManager();
	~CItemManager();

public:
	void Init();
	CItemRef FindItem(uint32 id) { return m_ItemMap[id]; }


private:
	std::unordered_map<uint32, CItemRef> m_ItemMap;
};


extern CItemManagerRef g_ItemManager;