#include "pch.h"
#include "GameObject.h"
#include "Component.h"
#include "RenderComponent.h"
#include "Script.h"

CGameObject::CGameObject()
	: m_arrComponent{}
	, m_RenderComponent(nullptr)
	, m_Parent(nullptr)
	, m_LayerIdx(-1)
{
}

CGameObject::~CGameObject()
{
	for (auto& component : m_arrComponent)
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

	// TODO: Layer ¿¡ GameObject µî·Ï

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
