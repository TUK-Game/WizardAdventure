#include "pch.h"
#include "Monster.h"
#include "StateManager.h"
#include "MonsterIdleState.h"
#include "MonsterChaseState.h"
#include "MonsterAttackState.h"
#include "Transform.h"
#include "Engine.h"
#include "MonsterAI.h"
#include "SkillManager.h"
#include "BoxCollider.h"
#include "CollisionManager.h"
#include "MeshData.h"
#include "AssetManager.h"
#include "MeshRenderer.h"

#include <iostream>

CMonster::CMonster()
{
	CMeshData* data2 = CAssetManager::GetInst()->FindAsset<CMeshData>(L"Crab");
	std::vector<CGameObject*> obj2 = data2->Instantiate(ECollision_Channel::Player); // temp

	CreateStateManager();
	AddComponent(new CMonsterAI);
	SetName(L"Crab");
	AddComponent(new CTransform);
	AddComponent(new CBoxCollider);
	GetCollider()->SetProfile(CCollisionManager::GetInst()->FindProfile("Player")); // temp
	//GetCollider()->SetMaxMinPos(Vec3(0, 0, 0), Vec3(100, 200, 24), Vec3(0, 0, 0), Vec3(0, 100, 0));
	GetTransform()->SetRelativePosition(11240, 20, 3000);
	for (auto& o : obj2)
	{
		std::wstring name = o->GetMeshRenderer()->GetMesh()->GetName();
		o->SetName(name);
		Vec3 rot = o->GetTransform()->GetRelativeRotation();
		o->GetTransform()->SetRelativeRotation(rot);
		//o->GetTransform()->SetRelativeScale(0.2f, 0.2f, 0.2f);
		o->SetCheckFrustum(true);
		o->SetInstancing(false);
		this->AddChild(o);
	}
}

CMonster::~CMonster()
{
}

void CMonster::Begin()
{
	CGameObject::Begin();
}

void CMonster::Update()
{
	if (m_StateManager) {
		m_StateManager->Update(this, DELTA_TIME);
	}
	CGameObject::Update();
}

void CMonster::FinalUpdate()
{
	CGameObject::FinalUpdate();
}

void CMonster::Render()
{
	CGameObject::Render();
}

void CMonster::CreateStateManager()
{
	m_StateManager = new CStateManager();
	m_StateManager->AddState(new CMonsterIdleState);
	m_StateManager->AddState(new CMonsterChaseState);
	m_StateManager->AddState(new CMonsterAttackState);

	m_StateManager->ChangeState(this, EState_Type::Idle);

	m_StateManager->SetTransition(EState_Type::Idle, "Chase", EState_Type::Chase);
	m_StateManager->SetTransition(EState_Type::Idle, "Attack", EState_Type::Attack);

	m_StateManager->SetTransition(EState_Type::Chase, "Attack", EState_Type::Attack);
	m_StateManager->SetTransition(EState_Type::Chase, "Idle", EState_Type::Idle);

	m_StateManager->SetTransition(EState_Type::Attack, "Idle", EState_Type::Idle);

}
