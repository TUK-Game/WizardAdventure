#pragma once

class CLayer;
class CGameObject;
class CSubLevel;

class CLevel :
	public CRef
{
public:
	CLevel();
	virtual ~CLevel();

public:
	CLayer* GetLayer(int index) const { return m_Layer[index]; }
	class CLevelCollision* GetLevelCollision() { return m_collision; }

	virtual void Deregister();

public:
	virtual void Init();
	virtual void Begin();
	virtual void Update();
	virtual void FinalUpdate();
	virtual void End();

public:
	void AddGameObject(CGameObject* object, int layerIndex, bool bChildMove);
	void SafeAddGameObject(CGameObject* object, int layerIndex, bool bChildMove);

	void RemoveGameObject(CGameObject* object);
	void RemoveGameObjectInLevel(CGameObject* object);

public:
	virtual CLevel* Clone() override	{ return new CLevel(*this); }

protected:
	std::array<CLayer*, MAX_LAYER>	m_Layer;
	class CLevelCollision* m_collision;
	
public:
	class CGameObject* m_MiniMap;
	std::shared_ptr<CSubLevel> m_SubLevel;
};

