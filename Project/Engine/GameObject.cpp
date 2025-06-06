﻿#include "pch.h"
#include "GameObject.h"
#include "LevelManager.h"
#include "Level.h"
#include "Layer.h"
#include "Component.h"
#include "RenderComponent.h"
#include "Transform.h"
#include "Script.h"
#include "ParticleSystem.h"
#include "Animator.h"
#include "Light.h"
#include "RigidBody.h"
#include "UI.h"
#include "UIButton.h"
#include "MeshRenderer.h"
#include "SubLevel.h"
#include "StateManager.h"
#include "Protocol.pb.h"
#include "Player.h"

CGameObject::CGameObject()
	: m_arrComponent{}
	, m_RenderComponent(nullptr)
	, m_Parent(nullptr)
	, m_LayerIndex(-1)
	, m_StateManager(nullptr)
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

	//for (auto& child : m_vecChild)
	//	delete child;

	if (m_StateManager)
		delete m_StateManager;

	// 부모가 있을 경우 부모의 child에서 자신을 제거
	/*if (m_Parent)
	{
		auto IsFind = std::find_if(m_Parent->m_vecChild.begin(), m_Parent->m_vecChild.end(), [&](CGameObject* obj) {
			return obj->GetName() == GetName();
		});

		if (IsFind != m_Parent->m_vecChild.end())
		{
			m_Parent->m_vecChild.erase(IsFind);
		}
	}*/
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

	auto iter = m_vecChild.begin();
	auto iterEnd = m_vecChild.end();
	for (; iter != iterEnd;)
	{
		if (!(*iter)->GetActive())
		{
			auto unActiveIter = *iter;
			iter = m_vecChild.erase(iter);
			iterEnd = m_vecChild.end();
			unActiveIter->ReleaseRef();
			continue;
		}
		else if ((*iter)->GetEnable())
		{
			(*iter)->Update();
		}

		++iter;
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
	//CLevel* curLevel = CLevelManager::GetInst()->GetCurrentLevel();
	//CLayer* layer = curLevel->GetLayer(m_LayerIndex);
	//if (m_LayerIndex == 3 || m_LayerIndex == 10)
	//{
	//	curLevel->m_SubLevel->RegisterGameObject(this, m_LayerIndex);
	//}
	//else
	//	layer->RegisterGameObject(this);

	for (size_t i = 0; i < m_vecChild.size(); ++i)
	{
		if (m_vecChild[i]->GetTransform())
			m_vecChild[i]->SetParentTransform(GetTransform());

		m_vecChild[i]->FinalUpdate();
	}
}

void CGameObject::Render()
{
	if (!m_RenderComponent || !m_bRender)
		return;

	m_RenderComponent->Render();
}

void CGameObject::CollisionBegin(CBaseCollider* src, CBaseCollider* dest)
{
}

CGameObject* CGameObject::GetRootObject()
{
	CGameObject* current = this;
	while (current->m_Parent)
	{
		current = current->m_Parent;
	}
	return current;
}

const CGameObject* CGameObject::GetRootObject() const
{
	const CGameObject* current = this;
	while (current->m_Parent)
	{
		current = current->m_Parent;
	}
	return current;
}

Protocol::MoveState CGameObject::GetStateForProtocol()
{
	auto type = m_StateManager->GetCurrentStateType();
	switch (type)
	{
	case EState_Type::Idle:
		return Protocol::MOVE_STATE_IDLE;
		break;
	case EState_Type::Run:
		return Protocol::MOVE_STATE_RUN;
		break;
	case EState_Type::Dash:
		return Protocol::MOVE_STATE_DASH;
		break;
	case EState_Type::Attack:
		break;
	case EState_Type::Attack_Q:
		return Protocol::MOVE_STATE_SKILL_Q;
		break;
	case EState_Type::Attack_R:
		return Protocol::MOVE_STATE_SKILL_R;
		break;
	case EState_Type::Attack_E:
		return Protocol::MOVE_STATE_SKILL_E;
		break;
	case EState_Type::Attack_LButton:
		return Protocol::MOVE_STATE_SKILL_MOUSE_L;
		break;
	case EState_Type::Attack_RButton:
		return Protocol::MOVE_STATE_SKILL_MOUSE_R;
		break;
	case EState_Type::Knockback:
		return Protocol::MOVE_STATE_DAMAGED;
	case EState_Type::Chase:
		break;
	case EState_Type::Hit:
		break;
	case EState_Type::Death:
		return Protocol::MOVE_STATE_DEATH;
		break;
	case EState_Type::Falling:
		return Protocol::MOVE_STATE_FALLING;
	case EState_Type::END:
		break;
	}
}

void CGameObject::SetParentTransform(CTransform* transform)
{
	GetTransform()->SetParentTransform(transform);
}

void CGameObject::SetProtocolStateForClient(Protocol::MoveState state)
{
	switch (state)
	{
	case Protocol::MOVE_STATE_NONE:
		break;
	case Protocol::MOVE_STATE_IDLE:
		m_StateManager->HandleEvent(this, "Stop");
		break;
	case Protocol::MOVE_STATE_RUN:
		m_StateManager->HandleEvent(this, "Move");
		break;
	case Protocol::MOVE_STATE_DASH:
		m_StateManager->HandleEvent(this, "Dash");
		break;
	case Protocol::MOVE_STATE_DASH_END:
		m_StateManager->HandleEvent(this, "EndDash");
		break;
	case Protocol::MOVE_STATE_SKILL_Q:
		m_StateManager->HandleEvent(this, "Attack_Q");
		break;
	case Protocol::MOVE_STATE_SKILL_E:
		m_StateManager->HandleEvent(this, "Attack_E");
		break;
	case Protocol::MOVE_STATE_SKILL_R:
		m_StateManager->HandleEvent(this, "Attack_R");
		break;
	case Protocol::MOVE_STATE_SKILL_MOUSE_R:
		m_StateManager->HandleEvent(this, "Attack_RButton");
		break;
	case Protocol::MOVE_STATE_SKILL_MOUSE_L:
		m_StateManager->HandleEvent(this, "Attack_LButton");
		break;
	case Protocol::MOVE_STATE_DAMAGED:
		m_StateManager->HandleEvent(this, "Knockback");
		break;
	case Protocol::MOVE_STATE_DAMAGED_END:
		m_StateManager->HandleEvent(this, "EndKnockback");
		break;
	case Protocol::MOVE_STATE_DEATH:
		m_StateManager->HandleEvent(this, "Death");
		break;
	case Protocol::MOVE_STATE_DEATH_END:
		m_StateManager->HandleEvent(this, "EndDeath");
		break;
	case Protocol::MOVE_STATE_FALLING:
		m_StateManager->HandleEvent(this, "Fall");
		break;
	case Protocol::MOVE_STATE_FALLING_END:
		static_cast<CPlayer*>(this)->SetDamageDelay(true);
		m_StateManager->HandleEvent(this, "EndFall");
		break;
	case Protocol::MOVE_STATE_DAMAGE_DELAY:
		static_cast<CPlayer*>(this)->SetDamageDelay(true);
		m_StateManager->HandleEvent(this, "EndKnockback");
		break;
	case Protocol::MOVE_STATE_DAMAGE_DELAY_END:
	{
		static_cast<CPlayer*>(this)->SetDamageDelay(false);
		static_cast<CPlayer*>(this)->SetIsRenderon(true);
		static_cast<CPlayer*>(this)->SetBlinkTime(0.f);
		m_StateManager->HandleEvent(this, "Move");

		const auto& childs = GetChild();
		for (auto& child : childs)
		{
			child->SetIsRender(true);
		}
	}
		break;
	default:
		break;
	}
}

void CGameObject::SetProtocolStateForClientMonster(Protocol::MoveState state)
{
	switch (state)
	{
	case Protocol::MOVE_STATE_NONE:
		break;
	case Protocol::MOVE_STATE_IDLE:
		m_StateManager->HandleEvent(this, "Idle");
		break;
	case Protocol::MOVE_STATE_RUN:
		m_StateManager->HandleEvent(this, "Chase");
		break;
	case Protocol::MOVE_STATE_DISSOVE:
		m_StateManager->HandleEvent(this, "Dissolve");
		break;
	case Protocol::MOVE_STATE_DASH_END:
		break;
	case Protocol::MOVE_STATE_SKILL_Q:
		m_StateManager->HandleEvent(this, "Attack");
		break;
	case Protocol::MOVE_STATE_DAMAGED:
		std::cout << "Damaged!!!" << std::endl;
		m_StateManager->HandleEvent(this, "Damaged");
		break;
	case Protocol::MOVE_STATE_DAMAGED_END:
		std::cout << "EndDamaged!!!" << std::endl;
		m_StateManager->HandleEvent(this, "EndDamaged");
		break;
	case Protocol::MOVE_STATE_DEATH:
		std::cout << "Death!!!" << std::endl;
		m_StateManager->HandleEvent(this, "Death");
		break;
	case Protocol::MOVE_STATE_SKILL_MOUSE_L:
		break;
	}
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

void CGameObject::AddComponent(EComponent_Type type)
{

	switch (type)
	{
	case EComponent_Type::Transform:
		AddComponent(new CTransform());
		break;
	case EComponent_Type::Camera:
		//AddComponent(new CCamera());
		break;
	case EComponent_Type::Collider:
		//AddComponent(new CBoxCollider());
		break;
	case EComponent_Type::Light:
		AddComponent(new CLight());
		break;
	case EComponent_Type::Animator:
		//AddComponent(new CAnimator());
		break;
	case EComponent_Type::Rigidbody:
		AddComponent(new CRigidBody());
		break;
	case EComponent_Type::UI:
		AddComponent(new CUI());
		break;
	case EComponent_Type::UIButton:
		AddComponent(new CUIButton());
		break;
	case EComponent_Type::MeshRenderer:
		AddComponent(new CMeshRenderer());
		break;
	case EComponent_Type::ParticleSystem:
		//AddComponent(new CTransform());
		break;
	case EComponent_Type::Skybox:
		//AddComponent(new CTransform());
		break;
	case EComponent_Type::Script:
		//AddComponent(new CTransform());
		break;
	default:
		break;
	}
}

void CGameObject::AddChild(CGameObject* obj)
{
	m_vecChild.push_back(obj);
	obj->SetParent(this);
}

void CGameObject::RemoveChild(CGameObject* child)
{
	m_vecChild.erase(std::remove(m_vecChild.begin(), m_vecChild.end(), child), m_vecChild.end());
}

void CGameObject::RemoveFromParent()
{
	if (m_Parent)
	{
		m_Parent->RemoveChild(this);
		m_Parent = nullptr;
	}
}

Vec3 CGameObject::InteractionCameraPos(Vec3& rot, const Vec3& offset)
{
	Vec3 pos = GetTransform()->GetRelativePosition();
	Vec3 front = GetTransform()->GetWorldDir(EDir::Front);
	Vec3 right = GetTransform()->GetWorldDir(EDir::Right);
	Vec3 up = GetTransform()->GetWorldDir(EDir::Up);
	front.Normalize();
	right.Normalize();
	up.Normalize();

	rot = GetTransform()->GetRelativeRotation();

	Vec3 newPos = pos - front * offset.z - right * offset.x - up * offset.y;

	return newPos;
}

void CGameObject::Destroy()
{
	// 모든 자식 오브젝트 삭제
	for (CGameObject* child : m_vecChild)
	{
		child->Destroy();
	}
	m_vecChild.clear();

	// 부모와의 연결 해제
	RemoveFromParent();

	CRef::Destroy();
}

