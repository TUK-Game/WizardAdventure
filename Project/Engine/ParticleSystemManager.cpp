#include "pch.h"
#include "ParticleSystemManager.h"
#include "Level.h"
#include "GameObject.h"
#include "ParticleSystem.h"
#include "Transform.h"

CParticleSystemManager::CParticleSystemManager()
{
}

CParticleSystemManager::~CParticleSystemManager()
{
}


void CParticleSystemManager::Init(int poolSize, CLevel* level)
{
	for (UINT32 i = 0; i < poolSize; ++i)
	{
		auto obj = new CGameObject();

		obj->AddComponent(new CParticleSystem);
		obj->AddComponent(new CTransform);
		obj->SetEnable(false); 

		level->AddGameObject(obj, 3 , false);
		m_Pool.push_back(obj);
	}
}

void CParticleSystemManager::Update(float deltaTime)
{
	for (auto it = m_ReturnQueue.begin(); it != m_ReturnQueue.end();)
	{
		it->remainingTime -= deltaTime;

		if (it->remainingTime <= 0.f)
		{
			it->obj->SetEnable(false);
			
			it = m_ReturnQueue.erase(it);
		}
		else
		{
			++it;
		}
	}
}

CGameObject* CParticleSystemManager::Request()
{
	for (auto obj : m_Pool)
	{
		if (!obj->GetEnable())
		{
			obj->GetParticleSystem()->SetEmit(true);
			obj->SetEnable(true);
			return obj;
		}
	}
	return nullptr;
}

void CParticleSystemManager::Return(CGameObject* obj)
{
	obj->GetParticleSystem()->SetEmit(false);
	m_ReturnQueue.push_back({ obj, 1.0f }); 
}

void CParticleSystemManager::Clear()
{
	for (auto obj : m_Pool)
	{
		if (obj)
		{
			delete obj;
			obj = nullptr;
		}
	}

	m_Pool.clear();
}