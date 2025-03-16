#include "pch.h"
#include "Level.h"
#include "Layer.h"
#include "GameObject.h"
#include "LevelCollision.h"

CLevel::CLevel()
	: m_Layer{}
{
	for (int i = 0; i < MAX_LAYER; ++i)
	{
		m_Layer[i] = new CLayer;
		m_Layer[i]->m_LayerIndex = i;
	}
	m_collision = new CLevelCollision();
}

CLevel::~CLevel()
{
	for (auto& layer : m_Layer)
		delete layer;
	delete m_collision;
	m_collision = nullptr;
}

void CLevel::Begin()
{
	for (auto& layer : m_Layer)
	{
		layer->Begin();
	}
}

void CLevel::Update()
{
	for (auto& layer : m_Layer)
	{
		layer->Update();
	}
}

void CLevel::FinalUpdate()
{
	for (auto& layer : m_Layer)
	{
		layer->FinalUpdate();
	}

	m_collision->Collision();
}

void CLevel::Deregister()
{
	for (auto& layer : m_Layer)
	{
		layer->m_vecObjects.clear();
	}
}

void CLevel::AddGameObject(CGameObject* object, int layerIndex, bool bChildMove)
{
	m_Layer[layerIndex]->AddGameObject(object, bChildMove);
}

void CLevel::RemoveGameObject(CGameObject* object)
{
	int layerNum = object->GetLayerIndex();
	m_Layer[layerNum]->RemoveGameObject(object);
}

void CLevel::End()
{
	for (auto& layer : m_Layer)
	{
		if (layer)
		{
			layer->ClearObjects(); // 각 레이어의 오브젝트 정리
		}
	}
}