#include "pch.h"
#include "Level_Start.h"
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
#include <Engine/Player.h>
#include <Engine/PlayerScript.h>
#include <Engine/Flag.h>
#include <Engine/Monster.h>
#include <Engine/BoxCollider.h>
#include <Engine/CollisionManager.h>
#include <Engine/CollisionObject.h>
#include <Engine/Device.h>
#include <Engine/RenderTargetGroup.h>
#include <Engine/SubLevel.h>
#include <Engine/BoxCollider.h>
#include <Engine/PlayWidgetWindow.h>
#include <Engine/Animator.h>
#include <Engine/TestWidget.h>
#include <Engine/TextWindow.h>

CLevel_Start::CLevel_Start()
{
	CLevelManager::GetInst()->SetLevel(this);
}

CLevel_Start::~CLevel_Start()
{
}

void CLevel_Start::Init()
{
	CLevel::Init();

	CAssetManager::GetInst()->LoadSound("BGM", "Play", false, "e.mp3");
	CAssetManager::GetInst()->SetVolume("BGM", 30);
	CAssetManager::GetInst()->SoundPlay("Play");

	CAssetManager::GetInst()->LoadSound("UI", "Click", false, "click.mp3");
	CAssetManager::GetInst()->SetVolume("UI", 30);


#pragma region UI_Camera
	{
		CGameObject* c = new CGameObject();
		c->SetName(L"Orthographic_Camera");
		c->AddComponent(new CTransform);
		c->AddComponent(new CCamera); // Near=1, Far=1000, 800*600
		c->GetCamera()->SetProjType(EProjection_Type::Orthographic);
		c->GetCamera()->SetPriority(1); // 0 : ���� ī�޶�� ����	
		c->GetCamera()->CheckLayerClear(); // �� ����
		c->GetCamera()->CheckLayer(4); // UI�� ����
		c->GetTransform()->SetRelativePosition(Vec3(0.f, 0.f, 0.f));
		this->AddGameObject(c, 0, false);
	}
#pragma endregion


	// directional light
	//{
	//	CGameObject* light = new CGameObject;
	//	light->SetName(L"DirectionalLight");
	//	light->AddComponent(new CTransform);
	//	light->AddComponent(new CLight);
	//	light->GetTransform()->SetRelativePosition(0.f, 5000.f, 100.f);
	//	light->GetLight()->SetLightDirection(Vec3(0.f, -1.f, 0.f));
	//	light->GetLight()->SetLightType(LIGHT_TYPE::DIRECTIONAL_LIGHT);
	//	light->GetLight()->SetDiffuse(Vec3(0.5f, 0.5f, 0.5f));
	//	light->GetLight()->SetAmbient(Vec3(0.7f, 0.7f, 0.7f));
	//	light->GetLight()->SetSpecular(Vec3(0.5f, 0.5f, 0.5f));
	//	CRenderManager::GetInst()->RegisterLight(light->GetLight());

	//	this->AddGameObject(light, 4, false);
	//}



	CGameObject* camera = new CGameObject;
	camera->SetName(L"MainCamera");
	camera->AddComponent(new CTransform);
	camera->AddComponent(new CCamera);
//	camera->GetCamera()->SetTarget(player);
//  camera->AddComponent(new CCameraScript);
	camera->GetCamera()->SetProjType(EProjection_Type::Perspective);
	camera->GetCamera()->SetPriority(0); // 0 : main camera
	camera->GetCamera()->CheckLayerAll();
	camera->GetCamera()->CheckLayer(4);
	camera->GetTransform()->SetRelativePosition(0.f, 0.f, 0.f);
	this->AddGameObject(camera, 0, false);


	// Main Texture
	CGameObject* mainTexture = new CGameObject;
	mainTexture->SetName(L"MainTexture");
	mainTexture->AddComponent(new CTransform);
	mainTexture->AddComponent(new CMeshRenderer);
	mainTexture->GetTransform()->SetRelativeScale(Vec3(2.f, 2.f, 1.f));
	mainTexture->GetMeshRenderer()->SetMesh(CAssetManager::GetInst()->FindAsset<CMesh>(L"Rectangle"));
	CMaterial* material = new CMaterial;
	CTexture* texture;
	texture = CAssetManager::GetInst()->FindAsset<CTexture>(L"Start");
	CGraphicShader* shader = CAssetManager::GetInst()->FindAsset<CGraphicShader>(L"Texture");
	material->SetTexture(0, texture);
	material->SetGraphicsShader(shader);
	mainTexture->GetMeshRenderer()->SetMaterial(material);
	this->AddGameObject(mainTexture, 4, false);


}

void CLevel_Start::Begin()
{
	CLevel::Begin();
	this->Update();
	this->FinalUpdate();
	CDevice::GetInst()->RenderBegin();
	CDevice::GetInst()->RenderEnd();
}

void CLevel_Start::Update()
{
	CLevel::Update();
}

void CLevel_Start::FinalUpdate()
{
	CLevel::FinalUpdate();
}

void CLevel_Start::End()
{
	CLevel::End();
}
