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
#include <Engine/ParticleSystemManager.h>

CLevel_1::CLevel_1()
{
	//CLevelManager::GetInst()->SetLevel(this);
}

CLevel_1::~CLevel_1()
{
	delete m_MiniMapBackground;
}

void CLevel_1::Init()
{
	CLevel::Init();

	m_MapSize = Vec3(8000, 3000, 7500);
	m_MapCenter = Vec3(4000, 0, 6500);
	m_SubLevel = std::make_shared<CSubLevel>();
	m_SubLevel->SetBoundingBox(m_MapCenter, m_MapSize);
	m_SubLevel->SplitSubScene(1);

	this->SetName(L"Level_1");

	this->GetLayer(0)->SetName(L"Camera");
	this->GetLayer(1)->SetName(L"BackGround");
	this->GetLayer(2)->SetName(L"Other");
	this->GetLayer(3)->SetName(L"Others");
	this->GetLayer(4)->SetName(L"UI");
	this->GetLayer(5)->SetName(L"Light");
	this->GetLayer(10)->SetName(L"Map");
	this->GetLayer(11)->SetName(L"Monster");	
	this->GetLayer(12)->SetName(L"Projectile");	

	CAssetManager::GetInst()->LoadSound("BGM", "Play", false, "e.mp3");
	CAssetManager::GetInst()->SetVolume("BGM", 30);
	CAssetManager::GetInst()->SoundPlay("Play");

	CAssetManager::GetInst()->LoadSound("UI", "Click", false, "click.mp3");
	CAssetManager::GetInst()->SetVolume("UI", 30);

#pragma endregion

	CParticleSystemManager::GetInst()->Init(10, this);

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
		light->GetTransform()->SetRelativePosition(6000.f, 10000.f, 6000.f);
		light->GetLight()->SetLightDirection(Vec3(0.f, -1.f, 0.f));
		light->GetLight()->SetLightType(LIGHT_TYPE::DIRECTIONAL_LIGHT);
		light->GetLight()->SetDiffuse(Vec3(0.5f, 0.5f, 0.5f));
		light->GetLight()->SetAmbient(Vec3(0.7f, 0.7f, 0.7f));
		light->GetLight()->SetSpecular(Vec3(0.5f, 0.5f, 0.5f));
		light->GetLight()->GetShadowCamera()->GetCamera()->SetFar(1000000);
		CRenderManager::GetInst()->RegisterLight(light->GetLight());

		this->AddGameObject(light, 4, false);
	}

#ifdef DEBUG_SOLOPLAY
	CPlayer* player = new CPlayer(EPlayerAttribute::Fire, true);
	this->AddGameObject(player, 3, false);
	CLevelManager::GetInst()->SetOwnPlayer(player);
#endif // DEBUG_SOLOPLAY

	//{
		//CMeshData* data2 = CAssetManager::GetInst()->FindAsset<CMeshData>(L"Crab");
		//std::vector<CGameObject*> obj2 = data2->Instantiate(ECollision_Channel::Player); // temp
		//CMonster* monster = new CMonster();
		//this->AddGameObject(monster, 11, false);
	//}

//
//
	CMeshData* data1 = CAssetManager::GetInst()->FindAsset<CMeshData>(L"level_1");
	std::vector<CGameObject*> obj1 = data1->Instantiate(ECollision_Channel::Wall);
	int m = 0;
	for (auto& o : obj1)
	{
		std::wstring name = o->GetMeshRenderer()->GetMesh()->GetName() + std::to_wstring(m++);
		o->SetName(name);

		//o->GetTransform()->SetRelativeScale(0.5f, 0.5f, 0.5f);
		Vec3 rot = o->GetTransform()->GetRelativeRotation();
		o->GetTransform()->SetRelativeRotation(rot);
		//o->GetTransform()->SetRelativePosition(100, 0, 0);
		//o->GetTransform()->SetRelativeScale(1, 1, 1);
		//o->AddComponent(new CTestPlayer);
		//o->GetMeshRenderer()->GetMaterial()->SetInt(0, 1);
		o->SetCheckFrustum(true);
		o->SetInstancing(true);
		this->AddGameObject(o, 10, false);

#ifdef COLLISION_MESH_DRAW
		CCollisionObject* co = new CCollisionObject();
		co->Init(o);
		co->GetCollider()->SetProfile(CCollisionManager::GetInst()->FindProfile("Wall"));
		co->SetInstancing(true);
		co->SetCheckFrustum(false);
		this->AddGameObject(co, 11, false);
#endif

	}

	CGameObject* camera = new CGameObject;
	camera->SetName(L"MainCamera");
	camera->AddComponent(new CTransform);
	camera->AddComponent(new CCamera);
#ifndef DEBUG_SOLOPLAY
	//camera->GetCamera()->SetCameraType(ECamera_Type::Fixed);
	//camera->GetCamera()->SetTarget(CLevelManager::GetInst()->GetOwnPlayer());
	camera->GetCamera()->SetFOV(60.f);
	camera->GetCamera()->SetFar(5000.f);
	camera->GetTransform()->SetRelativeRotation(49.f, -34.f, 0.f);
#else
	camera->GetCamera()->SetTarget(player);
#endif
	camera->AddComponent(new CCameraScript);
	camera->GetCamera()->SetProjType(EProjection_Type::Perspective);
	camera->GetCamera()->SetPriority(0); // 0 : main camera
	camera->GetCamera()->CheckLayerAll();
	camera->GetCamera()->CheckLayer(4);
	camera->GetTransform()->SetRelativePosition(0.f, 0.f, 0.f);
	this->AddGameObject(camera, 15, false);

#pragma region UI MAP

	CGameObject* mapCamera = new CGameObject;
	mapCamera->SetName(L"MiniMapCamera");
	mapCamera->AddComponent(new CTransform);
	mapCamera->AddComponent(new CCamera);
	mapCamera->GetCamera()->SetProjType(EProjection_Type::Perspective);
	mapCamera->GetCamera()->SetFOV(10.f); // 0 : main camera
	mapCamera->GetCamera()->SetPriority(2); // 0 : main camera
	mapCamera->GetCamera()->CheckLayerAll();
	mapCamera->GetCamera()->CheckLayer(4);
	mapCamera->GetCamera()->SetFar(100000);
	mapCamera->GetTransform()->SetRelativePosition(4919.f, 84667.f, 6754.f);
	mapCamera->GetTransform()->SetRelativeRotation(90.f, 0.f, 0.f);
	this->AddGameObject(mapCamera, 0, false);



#pragma endregion

#pragma region Widget

	CreateWidgetWindow<CPlayWidgetWindow>(EWIDGETWINDOW_TYPE::GAME_WINDOW, L"fuck2");
	CreateWidgetWindow<CTextWindow>(EWIDGETWINDOW_TYPE::TEXT_WINDOW, L"fuck");

#pragma endregion


#pragma region UI_TEST
	{
		CGameObject* obj = new CGameObject;
		obj->AddComponent(new CTransform);
		obj->AddComponent(new CMeshRenderer);
		//obj->GetTransform()->SetRelativeScale(Vec3(160.f, 160.f, 160.f));
		//obj->GetTransform()->SetRelativePosition(Vec3(-500.f + (i * 200), 250.f, 500.f));
		obj->GetTransform()->SetRelativeScale(Vec3(.2f, .35f, .2f));
		obj->GetTransform()->SetRelativePosition(Vec3(-0.8f , 0.5f, 1.0f));
		obj->GetMeshRenderer()->SetMesh(CAssetManager::GetInst()->FindAsset<CMesh>(L"Rectangle"));
		CMaterial* material = new CMaterial;
		CTexture* texture;

		texture = CDevice::GetInst()->GetRenderTargetGroup(RENDER_TARGET_GROUP_TYPE::SHADOW)->GetRTTexture(0);
		//texture = CAssetManager::GetInst()->FindAsset<CTexture>(L"UAVTexture");
		CGraphicShader* shader = CAssetManager::GetInst()->FindAsset<CGraphicShader>(L"Texture");
		material->SetTexture(0, texture);
		material->SetGraphicsShader(shader);
		obj->GetMeshRenderer()->SetMaterial(material);
		this->AddGameObject(obj, 4, false);
	}

#pragma endregion
}

void CLevel_1::Begin()
{
	CLevel::Begin();
	this->Update();
	this->FinalUpdate();
	m_MiniMapBackground = new CGameObject;
	CDevice::GetInst()->RenderBegin();
	CRenderManager::GetInst()->RenderMap(m_MiniMapBackground);
	CDevice::GetInst()->RenderEnd();
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
	CParticleSystemManager::GetInst()->Clear();
}
