#pragma once

class CGameObject;


class CLayer
{
	friend class CLevel;

public:
	CLayer();
	~CLayer();

public:
	const std::vector<CGameObject*>& GetObjects() const	{ return m_vecObjects; }

	void RegisterGameObject(CGameObject* object)	{ m_vecObjects.push_back(object); }
	void AddGameObject(CGameObject* parent, bool bChildMove);

public:
	void Begin();
	void Update();
	void FinalUpdate();

private:
	std::vector<CGameObject*>	m_vecParentObjects;	// �ֻ��� �θ� ������Ʈ
	std::vector<CGameObject*>	m_vecObjects;		// ��� ������Ʈ
	int							m_LayerIndex;		// ���̾� ��ȣ
};

