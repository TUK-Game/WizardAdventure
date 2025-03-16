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
	void RemoveGameObject(CGameObject* object);

public:
	void Begin();
	void Update();
	void FinalUpdate();
	void ClearObjects();

public:
	virtual CLayer* Clone() override { return new CLayer(*this); }

private:
	std::vector<CGameObject*>	m_vecParentObjects;	// 최상위 부모 오브젝트
	std::vector<CGameObject*>	m_vecObjects;		// 모든 오브젝트
	int							m_LayerIndex;		// 레이어 번호
};

