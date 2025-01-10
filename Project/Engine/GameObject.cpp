#include "pch.h"
#include "GameObject.h"
#include "LevelManager.h"
#include "Level.h"
#include "Layer.h"
#include "Component.h"
#include "RenderComponent.h"
#include "Script.h"

CGameObject::CGameObject()
	: m_arrComponent{}
	, m_RenderComponent(nullptr)
	, m_Parent(nullptr)
	, m_LayerIndex(-1)
{
}

CGameObject::~CGameObject()
{
	for (auto& component : m_arrComponent)
	{
		if (component)
			delete component;
	}

	for (auto& component : m_vecScript)
	{
		if (component)
			delete component;
	}

	for (auto& child : m_vecChild)
		delete child;
}

void CGameObject::Begin()
{
	for (int i = 0; i < (int)EComponent_Type::END; ++i)
	{
		if (m_arrComponent[i])
			m_arrComponent[i]->Begin();
	}

	for (size_t i = 0; i < m_vecScript.size(); ++i)
	{
		m_vecScript[i]->Begin();
	}

	for (size_t i = 0; i < m_vecChild.size(); ++i)
	{
		m_vecChild[i]->Begin();
	}
}

void CGameObject::Update()
{
	for (int i = 0; i < (int)EComponent_Type::END; ++i)
	{
		if (m_arrComponent[i])
			m_arrComponent[i]->Update();
	}

	for (size_t i = 0; i < m_vecScript.size(); ++i)
	{
		m_vecScript[i]->Update();
	}

	for (size_t i = 0; i < m_vecChild.size(); ++i)
	{
		m_vecChild[i]->Update();
	}
}

void CGameObject::FinalUpdate()
{
	for (int i = 0; i < (int)EComponent_Type::END; ++i)
	{
		if (m_arrComponent[i])
			m_arrComponent[i]->FinalUpdate();
	}

	// Layer 에 GameObject 등록
	CLevel* curLevel = CLevelManager::GetInst()->GetCurrentLevel();
	CLayer* layer = curLevel->GetLayer(m_LayerIndex);
	layer->RegisterGameObject(this);

	for (size_t i = 0; i < m_vecChild.size(); ++i)
	{
		m_vecChild[i]->FinalUpdate();
	}
}

void CGameObject::Render()
{
	if (!m_RenderComponent)
		return;

	m_RenderComponent->Render();
}

void CGameObject::AddComponent(CComponent* component)
{
	if (!component)
		return;

	EComponent_Type type = component->GetType();

	if (type == EComponent_Type::Script)
	{
		m_vecScript.push_back((CScript*)component);
	}
	else
	{
		// 이미 가지고 있는 컴포넌트인 경우
		assert(!m_arrComponent[(int)type]);
		m_arrComponent[(int)type] = component;

		if (dynamic_cast<CRenderComponent*>(component))
		{
			// 한 종류의 RenderComonent 만 가질 수 있다.
			if (m_RenderComponent)
				assert(nullptr);

			m_RenderComponent = (CRenderComponent*)component;
		}
	}

	component->m_Owner = this;
}