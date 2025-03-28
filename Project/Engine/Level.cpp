#include "pch.h"
#include "Level.h"
#include "Layer.h"
#include "GameObject.h"
#include "LevelCollision.h"
#include "SubLevel.h"
#include "WidgetWindow.h"

CLevel::CLevel()
	: m_Layer{}
{
	m_collision = new CLevelCollision();
}

CLevel::~CLevel()
{
	for (auto& layer : m_Layer)
		delete layer;
	delete m_collision;
	m_collision = nullptr;
}

void CLevel::Init()
{
	for (int i = 0; i < MAX_LAYER; ++i)
	{
		m_Layer[i] = new CLayer;
		m_Layer[i]->m_LayerIndex = i;
	}
}

void CLevel::Begin()
{
	for (int i = 0; i < MAX_LAYER; ++i)
	{
		if (i == 10)
		{
			m_SubLevel->Begin();
		}
		else
			m_Layer[i]->Begin();
	}
}

void CLevel::Update()
{
	for (int i = 0; i < MAX_LAYER; ++i)
	{
		if (i == 10)
		{
			m_SubLevel->Update();
		}
		else
			m_Layer[i]->Update();
	}

	for (auto& widget : m_vecWidgetWindow)
	{
		widget->Update();
	}
}

void CLevel::FinalUpdate()
{
	for (int i = 0; i < MAX_LAYER; ++i)
	{
		if (i == 10)
		{
			m_SubLevel->FinalUpdate();
		}
		else
			m_Layer[i]->FinalUpdate();
	}

	m_collision->Collision();

	for (auto& widget : m_vecWidgetWindow)
	{
		widget->FinalUpdate();
	}
}

void CLevel::Deregister()
{
	for (int i = 0; i < MAX_LAYER; ++i)
	{
		if (i == 10)
		{
			//m_SubLevel->Deregister();
		}
		else
			m_Layer[i]->m_vecObjects.clear();
	}
}

void CLevel::AddGameObject(CGameObject* object, int layerIndex, bool bChildMove)
{
	// layer가 오브젝트를 비추는 layer일 때
	if (layerIndex == 10)
	{
		bool b = m_SubLevel->AddGameObject(object, bChildMove);
	}
	else
		m_Layer[layerIndex]->AddGameObject(object, bChildMove);
}

void CLevel::SafeAddGameObject(CGameObject* object, int layerIndex, bool bChildMove)
{
	m_Layer[layerIndex]->SafeAddGameObject(object, bChildMove);
}

void CLevel::RemoveGameObject(CGameObject* object)
{
	if (!object) return;

	int layerNum = object->GetLayerIndex();
	if (layerNum == 10)
	{
		m_SubLevel->RemoveGameObject(object);
	}
	if (m_Layer[layerNum])
	{
		m_Layer[layerNum]->RemoveGameObject(object);
	}
}

void CLevel::RemoveGameObjectInLevel(CGameObject* object)
{
	if (!object) return;

	int layerNum = object->GetLayerIndex();
	if (m_Layer[layerNum])
	{
		m_Layer[layerNum]->RemoveGameObjectInLevel(object);
	}
}

void CLevel::End()
{
	for (int i = 0; i < MAX_LAYER; ++i)
	{
		if (i == 10)
		{
			m_SubLevel->End();
		}
		else
			if(m_Layer[i])
				m_Layer[i]->m_vecObjects.clear();
	}
}