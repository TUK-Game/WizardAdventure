#pragma once
#include "Level.h"

class CFrustum;

class CSubLevel :
	public CLevel
{
public:
	CSubLevel();
	virtual ~CSubLevel();

public:
	void SetBoundingBox(BoundingBox boundingBox) { m_BoundingBox = boundingBox; }
	void SetBoundingBox(Vec3 center, Vec3 extents) { m_BoundingBox = BoundingBox(center, extents); }
	BoundingBox& GetBoundingBox() { return m_BoundingBox; }
public:
	virtual void Init();
	virtual void Begin();
	virtual void Update();
	virtual void FinalUpdate();
	virtual void Deregister();
	virtual void End();

public:
	bool AddGameObject(CGameObject* object, int layerIndex, bool bChildMove);
	void RemoveGameObject(CGameObject* object);
	void RegisterGameObject(CGameObject* object, int);

	void PickGameObject(CFrustum& frustum, std::vector<CGameObject*>& objects, int layerIndex);
	void PickGameObject(std::vector<CGameObject*>& objects, int layerIndex);

	void SplitSubScene(int splitLevels);
public:
	virtual CSubLevel* Clone() override { return new CSubLevel(*this); }
	
	
	
public:
	BoundingBox					m_BoundingBox;
	std::vector<std::shared_ptr<CSubLevel>> m_SubLevels;
};

