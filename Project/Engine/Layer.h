#pragma once

#include "Ref.h"

class CGameObject;


class CLayer :
	public CRef
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

public:
	virtual CLayer* Clone() override { return new CLayer(*this); }

private:
	std::vector<CGameObject*>	m_vecParentObjects;	// �ֻ��� �θ� ������Ʈ
	std::vector<CGameObject*>	m_vecObjects;		// ��� ������Ʈ
	int							m_LayerIndex;		// ���̾� ��ȣ
};

