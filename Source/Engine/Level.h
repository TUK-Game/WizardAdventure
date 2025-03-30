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
	std::array<CSharedPtr<class CWidgetWindow>, (UINT)EWIDGETWINDOW_TYPE::END>& GetWidgetwindows() { return m_vecWidgetWindow; }
	CSharedPtr<class CWidgetWindow> GetWidgetWindow(EWIDGETWINDOW_TYPE type) { return m_vecWidgetWindow[(UINT)type]; }
	Vec3 GetMapSize() { return m_MapSize; }
	Vec3 GetMapCenter() { return m_MapCenter; }

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
	T* CreateWidgetWindow(EWIDGETWINDOW_TYPE type, const std::wstring& name)
	{
		if (m_vecWidgetWindow[(UINT)type])
			return nullptr;

		T* window = new T;
		window->SetName(name);
		if (!window->Init())
		{
			delete (window);
			return nullptr;
		}
		window->SetWindowType(type);

		m_vecWidgetWindow[(UINT)type] = window;

		return window;
	}

	CWidgetWindow* FindWidgetWindow(EWIDGETWINDOW_TYPE type)
	{
		if(m_vecWidgetWindow[(UINT)type])
			return m_vecWidgetWindow[(UINT)type];

		return nullptr;
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
	std::array<CLayer*, MAX_LAYER>												m_Layer;
	class CLevelCollision*														m_collision;
	std::array<CSharedPtr<class CWidgetWindow>, (UINT)EWIDGETWINDOW_TYPE::END>	m_vecWidgetWindow;
	Vec3																		m_MapSize;
	Vec3																		m_MapCenter;

public:
	std::shared_ptr<CSubLevel> m_SubLevel;
};

