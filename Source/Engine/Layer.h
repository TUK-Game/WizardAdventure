#pragma once

#include "Ref.h"

class CGameObject;

struct PendingAddObject
{
	CGameObject* parent;
	bool bChildMove;
};

class CLayer :
	public CRef
{
	friend class CLevel;
	friend class CSubLevel;

public:
	CLayer();
	~CLayer();

public:
	const std::vector<CGameObject*>& GetObjects() const	{ return m_vecObjects; }
	const std::vector<CGameObject*>& GetParentObjects() const	{ return m_vecParentObjects; }
	std::unordered_map<UINT64, class CMonster*>& GetMonsterMap() { return m_MonsterMap; }
	std::unordered_map<UINT64, class CSkillObject*>& GetProjectileMap() { return m_ProjectileMap; }

	void RegisterGameObject(CGameObject* object)	{ m_vecObjects.push_back(object); }
	void AddGameObject(CGameObject* parent, bool bChildMove);
	void SafeAddGameObject(CGameObject* parent, bool bChildMove);
	void SafeRemoveGameObject(CGameObject* obj);

	void FlushPendingObjects();
	void FlushPendingRemovals();

	void RemoveGameObject(CGameObject* object);
	void RemoveGameObjectInLevel(CGameObject* object);

public:
	void Begin();
	void Update();
	void FinalUpdate();
	void ClearObjects();
	void ClearObjectsVector();

public:
	virtual CLayer* Clone() override { return new CLayer(*this); }

private:
	std::vector<CGameObject*>	m_vecParentObjects;	// 최상위 부모 오브젝트
	std::vector<CGameObject*>	m_vecObjects;		// 모든 오브젝트
	std::vector<PendingAddObject> m_vecPendingAddObjects;
	std::vector<CGameObject*>	m_vecPendingRemoveObjects;

	std::unordered_map<UINT64, class CMonster*> m_MonsterMap;
	std::unordered_map<UINT64, class CSkillObject*> m_ProjectileMap;
	int							m_LayerIndex;		// 레이어 번호
};

