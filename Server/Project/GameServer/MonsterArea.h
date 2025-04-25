#pragma once
class CMonsterArea
{
public:
	std::vector<uint32>& GetMonstersId() { return m_Monsters; }
	std::vector<uint32>& GetGatesId() { return m_Gates; }

	void PushMonsterId(uint32 id) { m_Monsters.push_back(id); }
	void PushGateId(uint32 id) { m_Gates.push_back(id); }

private:
	std::vector<uint32> m_Monsters;
	std::vector<uint32> m_Gates;
};

