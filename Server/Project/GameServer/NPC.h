#pragma once
#include "Creature.h"

class CNPC : public CCreature
{
public:
	CNPC();
	virtual ~CNPC();

public:
	std::vector<CItemRef>& GetItemList() { return m_ItemList; }
	std::vector<CSkillRef>& GetSkillList() { return m_SkillList; }

	void PushItem(CItemRef item) { m_ItemList.push_back(item); }
	void PushSkill(CSkillRef skill) { m_SkillList.push_back(skill); }

private:
	std::vector<CItemRef> m_ItemList;
	std::vector<CSkillRef> m_SkillList;
};

