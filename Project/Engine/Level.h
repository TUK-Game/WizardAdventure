#pragma once
#include "WidgetWindow.h"

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
	std::vector<CSharedPtr<class CWidgetWindow>>& GetWidgetwindows() { return m_vecWidgetWindow; }

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
	template <typename T>
	T* CreateWidgetWindow(const std::wstring& name)
	{
		T* window = new T;
		window->SetName(name);
		if (!window->Init())
		{
			delete (window);
			return nullptr;
		}

		m_vecWidgetWindow.push_back(window);

		return window;
	}

	CWidget* FindWidget(const std::wstring& name)
	{
		for (size_t i = 0; i < m_vecWidgetWindow.size(); ++i)
		{
			CWidget* widget = m_vecWidgetWindow[i]->FindWidget(name);

			if (widget)
				return widget;
		}

		return nullptr;
	}

public:
	virtual CLevel* Clone() override	{ return new CLevel(*this); }

protected:
	std::array<CLayer*, MAX_LAYER>	m_Layer;
	class CLevelCollision* m_collision;
	std::vector<CSharedPtr<class CWidgetWindow>>	m_vecWidgetWindow;

public:
	std::shared_ptr<CSubLevel> m_SubLevel;
};

