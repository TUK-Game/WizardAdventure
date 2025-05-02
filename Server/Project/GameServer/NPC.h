#pragma once
#include "Creature.h"

class CNPC : public CCreature
{
public:
	CNPC();
	virtual ~CNPC();

public:
	std::vector<CItemRef>& GetItemList() { return m_ItemList; }

	void PushItem(CItemRef item) { m_ItemList.push_back(item); }

private:
	std::vector<CItemRef> m_ItemList;
};

