#include "pch.h"
#include "Layer.h"
#include "GameObject.h"

CLayer::CLayer()
	: m_LayerIndex(-1)
{
}

CLayer::~CLayer()
{
	ClearObjects(); 
}

void CLayer::AddGameObject(CGameObject* parent, bool bChildMove)
{
	m_vecParentObjects.push_back(parent);
	parent->m_LayerIndex = m_LayerIndex;
	// 자식이 있다면, 자식도 해당 레이어 소속으로 변경
	static std::list<CGameObject*> queue;
	queue.clear();
	queue.push_back(parent);

	while (!queue.empty())
	{
		CGameObject* pObject = queue.front();
		queue.pop_front();

		const std::vector<CGameObject*>& vecChild = pObject->GetChild();
		for (size_t i = 0; i < vecChild.size(); ++i)
		{
			queue.push_back(vecChild[i]);
		}

		// case 1. 최상위 부모 오브젝트
		// case 2. 자식오브젝트지만 자식까지 같이 이동하기로 한 경우 
		// case 3. 최상위부모오브젝트 X, 자식이동 X 라도 소속이 없는 오브젝트인 경우
		if (!pObject->GetParent() || bChildMove || pObject->m_LayerIndex == -1)
		{
			pObject->m_LayerIndex = m_LayerIndex;
		}
		m_vecObjects.push_back(pObject);
	}
}

void CLayer::SafeAddGameObject(CGameObject* parent, bool bChildMove)
{
	m_vecPendingAddObjects.push_back({ parent, bChildMove });
	parent->m_LayerIndex = m_LayerIndex;
}

void CLayer::SafeRemoveGameObject(CGameObject* obj)
{
	m_vecPendingRemoveObjects.push_back(obj);
}

void CLayer::FlushPendingObjects()
{
	for (const PendingAddObject& item : m_vecPendingAddObjects)
	{
		AddGameObject(item.parent, item.bChildMove);
	}
	m_vecPendingAddObjects.clear();
}

void CLayer::FlushPendingRemovals()
{
	for (CGameObject* obj : m_vecPendingRemoveObjects)
	{
		RemoveGameObject(obj);
	}
	m_vecPendingRemoveObjects.clear();
}

void CLayer::RemoveGameObject(CGameObject* object)
{
	if (!object) return;

	// 객체 삭제 전 부모-자식 관계 정리
	object->Destroy(); // 모든 자식 삭제 + 부모 관계 해제

	// 모든 오브젝트 벡터에서 제거
	m_vecObjects.erase(std::remove(m_vecObjects.begin(), m_vecObjects.end(), object), m_vecObjects.end());

	// 최상위 부모 오브젝트 벡터에서도 제거
	m_vecParentObjects.erase(std::remove(m_vecParentObjects.begin(), m_vecParentObjects.end(), object), m_vecParentObjects.end());

	if (object) 
		delete object;
}

void CLayer::RemoveGameObjectInLevel(CGameObject* object)
{
	if (!object) return;

	// 모든 오브젝트 벡터에서 제거
	m_vecObjects.erase(std::remove(m_vecObjects.begin(), m_vecObjects.end(), object), m_vecObjects.end());

	// 최상위 부모 오브젝트 벡터에서도 제거
	m_vecParentObjects.erase(std::remove(m_vecParentObjects.begin(), m_vecParentObjects.end(), object), m_vecParentObjects.end());
}


void CLayer::Begin()
{
	for (auto& object : m_vecParentObjects)
	{
		object->Begin();
	}
}

void CLayer::Update()
{
	auto iter = m_vecParentObjects.begin();
	auto iterEnd = m_vecParentObjects.end();
	for (; iter != iterEnd;)
	{
		if (!(*iter)->GetActive())
		{
			delete* iter;
			iter = m_vecParentObjects.erase(iter);
			iterEnd = m_vecParentObjects.end();
			continue;
		}
		else if ((*iter)->GetEnable())
		{
			(*iter)->Update();
		}

		++iter;
	}
}

void CLayer::FinalUpdate()
{
	for (auto& object : m_vecParentObjects)
	{
		object->FinalUpdate();
	}
	FlushPendingObjects();
	FlushPendingRemovals();
}

void CLayer::ClearObjects()
{
	for (auto& object : m_vecParentObjects)
	{
		delete object; 
	}
	m_vecParentObjects.clear(); 
	m_vecObjects.clear();       
}

void CLayer::ClearObjectsVector()
{
	m_vecParentObjects.clear();
	m_vecObjects.clear();
}
