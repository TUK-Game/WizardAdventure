#pragma once
class CItemManager :
	public CSingleton<CItemManager>
{
	DECLARE_SINGLETON(CItemManager)

public:
	bool Init();
	std::shared_ptr<class CItem> FindItem(uint32 id) { return m_ItemList[id]; }
	std::unordered_map<uint32, std::shared_ptr<class CItem>>& GetItemList() { return m_ItemList; }

private:
	std::unordered_map<uint32, std::shared_ptr<class CItem>> m_ItemList;
};

