#pragma once
#include "Ref.h"

class CFrustum;
class CGameObject;

class CSubLevel :
	public CRef
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
	virtual void End();

public:
	bool AddGameObject(CGameObject* object, bool bChildMove);
	void Insert(CGameObject* object, bool bChildMove);
	void RemoveGameObject(CGameObject* object);

	void PickGameObject(CFrustum& frustum, std::vector<CGameObject*>& objects);
	void PickGameObject(std::vector<CGameObject*>& objects);

	void SplitSubScene(int splitLevels);
public:
	virtual CSubLevel* Clone() override { return new CSubLevel(*this); }
	
	
	
public:
	BoundingBox								m_BoundingBox;
	std::vector<std::shared_ptr<CSubLevel>> m_SubLevels;
	class CLayer*							m_Layer;
};

