#include "pch.h"
#include "NPC.h"
#include "AssetManager.h"
#include "MeshData.h"
#include "Transform.h"
#include "BoxCollider.h"
#include "CollisionManager.h"
#include "MeshRenderer.h"
#include "StateManager.h"
#include "Engine.h"
#include "NPCBuyState.h"
#include "NPCIdleState.h"
#include "NPCTalkState.h"
#include "LevelManager.h"
#include "Level.h"

CNPC::CNPC()
{
	CMeshData* data = CAssetManager::GetInst()->FindAsset<CMeshData>(L"StoreNPC");
	std::vector<CGameObject*> obj = data->Instantiate(ECollision_Channel::Player); // temp

	CreateStateManager();
	//AddComponent(new CMonsterAI);
	SetName(L"Crab");
	AddComponent(new CTransform);
	AddComponent(new CBoxCollider);
	GetCollider()->SetProfile(CCollisionManager::GetInst()->FindProfile("Player")); // temp
	GetCollider()->SetMaxMinPos(Vec3(11000, 20, 3500), Vec3(100, 200, 24), Vec3(0, 0, 0), Vec3(0, 100, 0));
	GetTransform()->SetRelativePosition(11000, 20, 3500);
	for (auto& o : obj)
	{
		std::wstring name = o->GetMeshRenderer()->GetMesh()->GetName();
		o->SetName(name);
		Vec3 rot = o->GetTransform()->GetRelativeRotation();
		o->GetTransform()->SetRelativeRotation(rot);
		//o->GetTransform()->SetRelativeScale(0.2f, 0.2f, 0.2f);
		o->SetCheckFrustum(false);
		o->SetInstancing(false);
		this->AddChild(o);
	}

}

CNPC::~CNPC()
{
}

void CNPC::Begin()
{
	CGameObject::Begin();
	m_StateManager->ChangeState(this, EState_Type::Idle);
}

void CNPC::Update()
{
	float time = DELTA_TIME;
	if (m_StateManager) {
		m_StateManager->Update(this, time);
	}

	CGameObject::Update();
}

void CNPC::FinalUpdate()
{
	CGameObject::FinalUpdate();
}

void CNPC::Render()
{
	CGameObject::Render();
}

void CNPC::CreateStateManager()
{
	m_StateManager = new CStateManager();
	m_StateManager->AddState(new CNPCIdleState);
	m_StateManager->AddState(new CNPCTalkState);
	m_StateManager->AddState(new CNPCBuyState);


	m_StateManager->SetTransition(EState_Type::Idle, "Talk", EState_Type::Talk);

	m_StateManager->SetTransition(EState_Type::Talk, "Buy", EState_Type::Buy);
	m_StateManager->SetTransition(EState_Type::Talk, "Finish", EState_Type::Idle);

	m_StateManager->SetTransition(EState_Type::Buy, "FinishStore", EState_Type::Idle);

}

Vec3 CNPC::InteractionCameraPos(Vec3& rot)
{
	Vec3 pos = GetTransform()->GetRelativePosition();
	Vec3 front = GetTransform()->GetWorldDir(EDir::Front);
	Vec3 right = GetTransform()->GetWorldDir(EDir::Right);
	front.Normalize();
	right.Normalize();

	rot = GetTransform()->GetRelativeRotation();

	Vec3 newPos = pos - front * 500 - right * 200;

	return newPos;
}

void CNPC::Interation()
{
	m_StateManager->HandleEvent(this, "Talk");
	ShowWidgetWindow();
}

void CNPC::ShowWidgetWindow()
{
	const auto& window = CLevelManager::GetInst()->GetCurrentLevel()->FindWidgetWindow(m_WindowType);
	if (window)
	{
		window->SetEnable(true);
	}
}
