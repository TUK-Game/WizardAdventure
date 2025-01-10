#include "pch.h"
#include "LevelManager.h"
#include "AssetManager.h"
#include "Level.h"
#include "Layer.h"
#include "GameObject.h"
#include "Camera.h"
#include "Transform.h"
#include "MeshRenderer.h"
#include "CameraScript.h"

CLevelManager::CLevelManager()
	: m_CurLevel(nullptr)
{
}

CLevelManager::~CLevelManager()
{
	delete m_CurLevel;
}

int CLevelManager::Init()
{
	m_CurLevel = new CLevel;

	// example
	m_CurLevel->GetLayer(0)->SetName(L"Default");
	m_CurLevel->GetLayer(1)->SetName(L"Background");
	m_CurLevel->GetLayer(2)->SetName(L"Player");
	m_CurLevel->GetLayer(3)->SetName(L"Enermy");

	// 카메라 역할 오브젝트 생성
	CGameObject* camera = new CGameObject;
	camera->SetName(L"MainCamera");
	camera->AddComponent(new CTransform);
	camera->AddComponent(new CCamera);
	camera->AddComponent(new CCameraScript);
	camera->GetCamera()->SetProjType(EProjection_Type::Perspective);
	camera->GetCamera()->SetPriority(0); // 0 : 메인 카메라로 설정
	camera->GetCamera()->CheckLayerAll();
	camera->GetCamera()->CheckLayer(31);
	camera->GetTransform()->SetRelativePosition(0.f, 0.f, 0.f);
	m_CurLevel->AddGameObject(camera, 0, false);

	CGameObject* object = new CGameObject;
	object->SetName(L"Cube");
	object->AddComponent(new CTransform);
	object->AddComponent(new CMeshRenderer);
	object->GetTransform()->SetRelativeScale(100.f, 100.f, 100.f);
	object->GetTransform()->SetRelativeRotation(0.f, 0.f, 0.f);
	object->GetTransform()->SetRelativePosition(0.f, 0.f, 300.f);
	object->GetMeshRenderer()->SetMesh(CAssetManager::GetInst()->FindAsset<CMesh>(L"Cube"));
	object->GetMeshRenderer()->SetMaterial(CAssetManager::GetInst()->FindAsset<CMaterial>(L"Kita"));
	m_CurLevel->AddGameObject(object, 0, false);

	m_CurLevel->Begin();

	return S_OK;
}

void CLevelManager::Progress()
{
	m_CurLevel->Update();

	m_CurLevel->Deregister();

	m_CurLevel->FinalUpdate();
}
