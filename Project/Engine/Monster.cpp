#include "pch.h"
#include "Monster.h"
#include "StateManager.h"
#include "MonsterIdleState.h"
#include "MonsterChaseState.h"
#include "MonsterAttackState.h"
#include "MonsterDamagedState.h"
#include "MonsterSpawnState.h"
#include "MonsterDeathState.h"
#include "MonsterDissolveState.h"
#include "Transform.h"
#include "Engine.h"
#include "MonsterAI.h"
#include "SkillManager.h"
#include "BoxCollider.h"
#include "CollisionManager.h"
#include "MeshData.h"
#include "AssetManager.h"
#include "MeshRenderer.h"
#include "OrientedBoxCollider.h"
#include "MonsterHPBar.h"

#include <iostream>

CMonster::CMonster()
	: m_Interpolator(new CInterpolator())
{

	CMeshData* data2;
	std::vector<CGameObject*> obj2;
	//if(m_Type == EMonsterType::Crab)
	{ 
		data2 = CAssetManager::GetInst()->FindAsset<CMeshData>(L"Crab");
		obj2 = data2->Instantiate(ECollision_Channel::Player); // temp
	}

	CreateStateManager();	
	//AddComponent(new CMonsterAI);
	SetName(L"Crab");
	AddComponent(new CTransform);
	AddComponent(new COrientedBoxCollider);
	GetCollider()->SetProfile(CCollisionManager::GetInst()->FindProfile("Player")); // temp

	Vec3 ms;
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
		ms = o->GetMeshRenderer()->GetMesh()->GetMeshSize();
	}
	GetCollider()->SetMaxMinPos(GetTransform()->GetRelativePosition(), ms * 2, Vec3(0, 0, 0), Vec3(0, 100, 0));

	m_Stats = new Stats;

	// hpbar
	CMonsterHPBar* hpBar = new CMonsterHPBar();
	hpBar->SetOffsetY(450.f);
	AddChild(hpBar);

	// temp
	GetStat()->gold = 100.f;

	Begin();	
}

CMonster::~CMonster()
{
	if (m_Interpolator)
		delete m_Interpolator;
	if (m_Stats)
		delete m_Stats;
}

void CMonster::Begin()
{
	CGameObject::Begin();
	m_StateManager->ChangeState(this, EState_Type::Spawn);
	//m_Interpolator->SetTarget(GetTransform()->GetRelativePosition(), GetTransform()->GetRelativeRotation());
}

void CMonster::Update()
{
	float time = DELTA_TIME;
	if (m_StateManager) {
		m_StateManager->Update(this, time);
	}
#ifndef DEBUG_SOLOPLAY
	m_Interpolator->Update(time);
	GetTransform()->SetRelativePosition((m_Interpolator->GetInterpolatedPos()));
	GetTransform()->SetRelativeRotation((m_Interpolator->GetInterpolatedRot()));
#endif // DEBUG_SOLOPLAY

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
	m_StateManager->AddState(new CMonsterDamagedState);
	m_StateManager->AddState(new CMonsterDeathState);
	m_StateManager->AddState(new CMonsterSpawnState);
	m_StateManager->AddState(new CMonsterDissolveState);

	// spawn -> idle
	m_StateManager->SetTransition(EState_Type::Spawn, "Idle", EState_Type::Idle);

	// idle -> others
	m_StateManager->SetTransition(EState_Type::Idle, "Chase", EState_Type::Chase);
	m_StateManager->SetTransition(EState_Type::Idle, "Attack", EState_Type::Attack);
	m_StateManager->SetTransition(EState_Type::Idle, "Damaged", EState_Type::Damaged);
	m_StateManager->SetTransition(EState_Type::Idle, "Death", EState_Type::Death);

	// damaged -> others
	m_StateManager->SetTransition(EState_Type::Damaged, "EndDamaged", EState_Type::Idle);
	m_StateManager->SetTransition(EState_Type::Damaged, "Death", EState_Type::Death);

	// chase -> others
	m_StateManager->SetTransition(EState_Type::Chase, "Attack", EState_Type::Attack);
	m_StateManager->SetTransition(EState_Type::Chase, "Idle", EState_Type::Idle);
	m_StateManager->SetTransition(EState_Type::Chase, "Damaged", EState_Type::Damaged);
	m_StateManager->SetTransition(EState_Type::Chase, "Death", EState_Type::Death);

	// attack -> others
	m_StateManager->SetTransition(EState_Type::Attack, "Idle", EState_Type::Idle);
	//m_StateManager->SetTransition(EState_Type::Attack, "Damaged", EState_Type::Damaged);
	m_StateManager->SetTransition(EState_Type::Attack, "Death", EState_Type::Death);

	// death -> dissolve
	m_StateManager->SetTransition(EState_Type::Death, "Dissolve", EState_Type::Dissolve);
}

void CMonster::ReceiveDamage(int damage)
{
	m_Stats->TakeDamage(damage);
	if (m_Stats->IsDead()) {
		OnDeath();
	}

}

void CMonster::OnDeath()
{	
}
