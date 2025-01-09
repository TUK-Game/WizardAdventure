#include "pch.h"
#include "LevelManager.h"
#include "AssetManager.h"
#include "Level.h"
#include "GameObject.h"
#include "Camera.h"
#include "Transform.h"
#include "MeshRenderer.h"

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

	// 카메라 역할 오브젝트 생성
	CGameObject* camera = new CGameObject;
	camera->SetName(L"MainCamera");
	camera->AddComponent(new CTransform);
	camera->AddComponent(new CCamera);
	camera->GetCamera()->SetProjType(EProjection_Type::Perspective);
	camera->GetCamera()->SetPriority(0); // 0 : 메인 카메라로 설정
	camera->GetCamera()->CheckLayerAll();
	camera->GetCamera()->CheckLayer(31);
	camera->GetTransform()->SetRelativePosition(0.f, 100.f, -100.f);
	m_CurLevel->AddGameObject(camera, 0, false);

	CGameObject* object = new CGameObject;
	object->SetName(L"Cube");
	object->AddComponent(new CTransform);
	object->AddComponent(new CMeshRenderer);
	object->GetTransform()->SetRelativeScale(100.f, 100.f, 100.f);
	object->GetTransform()->SetRelativeRotation(-30.f, 30.f, 0.f);
	object->GetTransform()->SetRelativePosition(0.f, 100.f, 200.f);
	object->GetMeshRenderer()->SetMesh(CAssetManager::GetInst()->FindAsset<CMesh>(L"Cube"));
	// TODO: object->GetMeshRenderer()->SetMaterial();
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
