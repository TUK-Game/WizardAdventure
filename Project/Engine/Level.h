#pragma once

class CLayer;
class CGameObject;


class CLevel :
	public CRef
{
public:
	CLevel();
	~CLevel();

public:
	CLayer* GetLayer(int index) const { return m_Layer[index]; }
	class CLevelCollision* GetLevelCollision() { return m_collision; }

	void Deregister();

public:
	void Begin();
	void Update();
	void FinalUpdate();
	void End();

public:
	void AddGameObject(CGameObject* object, int layerIndex, bool bChildMove);
	void RemoveGameObject(CGameObject* object);

public:
	virtual CLevel* Clone() override	{ return new CLevel(*this); }

private:
	std::array<CLayer*, MAX_LAYER>	m_Layer;
	class CLevelCollision* m_collision;

};

