#pragma once

class CGameObject;


class CLayer
{
	friend class CLevel;

public:
	CLayer();
	~CLayer();

public:
	void Begin();
	void Update();
	void FinalUpdate();

private:
	std::vector<CGameObject*>	m_vecObjects;
	int							m_LayerIndex;	// 레이어 번호
};

