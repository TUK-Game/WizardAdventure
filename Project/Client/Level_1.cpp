#include "pch.h"
#include "Level_1.h"
#include <Engine/AssetManager.h>
#include <Engine/Camera.h>
#include <Engine/Transform.h>
#include <Engine/CameraScript.h>
#include <Engine/MeshRenderer.h>
#include <Engine/Light.h>
#include <Engine/RenderManager.h>
#include <Engine/UIButton.h>
#include <Engine/TestPlayer.h>
#include <Engine/LevelManager.h>
#include <Engine/Layer.h>
#include <Engine/MeshData.h>
#include <Engine/UIButton.h>

CLevel_1::CLevel_1()
{
	CLevelManager::GetInst()->SetLevel(this);
}

CLevel_1::~CLevel_1()
{
}

void CLevel_1::Init()
{
	this->SetName(L"Level_1");

	this->GetLayer(0)->SetName(L"Camera");
	this->GetLayer(1)->SetName(L"BackGround");
	this->GetLayer(2)->SetName(L"Other");
	this->GetLayer(3)->SetName(L"Others");
	this->GetLayer(4)->SetName(L"UI");
	this->GetLayer(5)->SetName(L"Light");
	this->GetLayer(10)->SetName(L"Map");

	CAssetManager::GetInst()->LoadSound("BGM", "Play", false, "e.mp3");
	CAssetManager::GetInst()->SetVolume("BGM", 30);
	CAssetManager::GetInst()->SoundPlay("Play");

	CAssetManager::GetInst()->LoadSound("UI", "Click", false, "click.mp3");
	CAssetManager::GetInst()->SetVolume("UI", 30);

#pragma endregion

	// 카메라 역할 오브젝트 생성
	CGameObject* camera = new CGameObject;
	camera->SetName(L"MainCamera");
	camera->AddComponent(new CTransform);
	camera->AddComponent(new CCamera);
	camera->AddComponent(new CCameraScript);
	camera->GetCamera()->SetProjType(EProjection_Type::Perspective);
	camera->GetCamera()->SetPriority(0); // 0 : 메인 카메라로 설정	
	camera->GetCamera()->CheckLayerAll();
	camera->GetCamera()->CheckLayer(4);
	camera->GetTransform()->SetRelativePosition(0.f, 0.f, 0.f);
	this->AddGameObject(camera, 0, false);

#pragma region UI_Camera
	{
		CGameObject* c = new CGameObject();
		c->SetName(L"Orthographic_Camera");
		c->AddComponent(new CTransform);
		c->AddComponent(new CCamera); // Near=1, Far=1000, 800*600
		c->GetCamera()->SetProjType(EProjection_Type::Orthographic);
		c->GetCamera()->SetPriority(1); // 0 : 메인 카메라로 설정	
		c->GetCamera()->CheckLayerClear(); // 다 끄고
		c->GetCamera()->CheckLayer(4); // UI만 찍음
		c->GetTransform()->SetRelativePosition(Vec3(0.f, 0.f, 0.f));
		this->AddGameObject(c, 0, false);
	}
#pragma endregion

	CGameObject* skybox = new CGameObject;
	skybox->SetName(L"Skybox");
	skybox->AddComponent(new CTransform);
	skybox->AddComponent(new CMeshRenderer);
	skybox->GetMeshRenderer()->SetMesh(CAssetManager::GetInst()->FindAsset<CMesh>(L"Cube"));
	skybox->GetMeshRenderer()->SetMaterial(CAssetManager::GetInst()->FindAsset<CMaterial>(L"Skybox"));
	this->AddGameObject(skybox, 1, false);


	// directional light
	{
		CGameObject* light = new CGameObject;
		light->SetName(L"DirectionalLight");
		light->AddComponent(new CTransform);
		light->AddComponent(new CLight);
		light->GetTransform()->SetRelativePosition(0.f, 500.f, 100.f);
		light->GetLight()->SetLightDirection(Vec3(0.f, -1.f, 0.5f));
		light->GetLight()->SetLightType(LIGHT_TYPE::DIRECTIONAL_LIGHT);
		light->GetLight()->SetDiffuse(Vec3(0.5f, 0.5f, 0.5f));
		light->GetLight()->SetAmbient(Vec3(0.1f, 0.1f, 0.0f));
		light->GetLight()->SetSpecular(Vec3(0.5f, 0.5f, 0.5f));
		CRenderManager::GetInst()->RegisterLight(light->GetLight());

		this->AddGameObject(light, 3, false);
	}

	CMeshData* data = CAssetManager::GetInst()->FindAsset<CMeshData>(L"Mage");
	std::vector<CGameObject*> obj = data->Instantiate();
	CGameObject* player = new CGameObject;
	player->SetName(L"Mage");
	player->AddComponent(new CTransform);
	for (auto& o : obj)
	{
		std::wstring name = o->GetMeshRenderer()->GetMesh()->GetName();
		o->SetName(name);

		//o->GetTransform()->SetRelativeScale(0.5f, 0.5f, 0.5f);
		Vec3 rot = o->GetTransform()->GetRelativeRotation();
		rot.x += -90;
		o->GetTransform()->SetRelativeRotation(rot);
		//o->GetTransform()->SetRelativePosition(100, 0, 0);
		//o->GetTransform()->SetRelativeScale(1, 1, 1);
		o->AddComponent(new CTestPlayer);
		//o->GetMeshRenderer()->GetMaterial()->SetInt(0, 1);
		o->SetCheckFrustum(true);
		player->AddChild(o);
	}
	this->AddGameObject(player, 10, false);
}

void CLevel_1::Begin()
{
	CLevel::Begin();
}

void CLevel_1::Update()
{
	CLevel::Update();
}

void CLevel_1::FinalUpdate()
{
	CLevel::FinalUpdate();
}

void CLevel_1::End()
{
	CLevel::End();
}
