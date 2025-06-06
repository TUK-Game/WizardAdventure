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
#include <Engine/PlayWidgetWindow.h>
#include <Engine/Animator.h>
#include <Engine/TestWidget.h>
#include <Engine/TextWindow.h>
#include <Engine/ParticleSystemManager.h>
#include <Engine/AnimatedBillboardEffect.h>
#include <Engine/EffectManager.h>
#include <Engine/NetworkManager.h>
#include <Engine/ServerSession.h>
#include <Engine/Engine.h>
#include <Engine/NPC.h>
#include <Engine/Portal.h>
#include <Engine/ParticleSystem.h>

CLevel_1::CLevel_1()
{
	//CLevelManager::GetInst()->SetLevel(this);
}

CLevel_1::CLevel_1(EPlayerAttribute attribute)
	: m_Attribute(attribute)
{
	//CLevelManager::GetInst()->SetLevel(this);
}

CLevel_1::~CLevel_1()
{
	delete m_MiniMapBackground;
}

void CLevel_1::SelectMage(EPlayerAttribute attribute)
{

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
	this->GetLayer(13)->SetName(L"Effect");
	this->GetLayer(12)->SetName(L"Gate");	
	this->GetLayer(15)->SetName(L"NPC");	

	CAssetManager::GetInst()->LoadSound("BGM", "Play", false, "e.mp3");
	CAssetManager::GetInst()->LoadSound("BGM", "Level_1_BGM", true, "Level_1_BGM.mp3");
	CAssetManager::GetInst()->SetVolume("BGM", 100);
	CAssetManager::GetInst()->SoundPlay("Level_1_BGM");

	CAssetManager::GetInst()->LoadSound("UI", "Click", false, "click.mp3");
	CAssetManager::GetInst()->SetVolume("UI", 30);

	CAssetManager::GetInst()->LoadSound("EFFECT", "Damaged", false, "Damaged.mp3");
	CAssetManager::GetInst()->LoadSound("EFFECT", "Move", false, "Move.mp3");
	CAssetManager::GetInst()->LoadSound("SKILL", "FireBall", false, "FireBall.mp3");
	CAssetManager::GetInst()->LoadSound("SKILL", "FireTower", false, "FireTower.mp3");
	CAssetManager::GetInst()->LoadSound("SKILL", "FireTowerDissolve", false, "FireTowerDissolve.mp3");
	CAssetManager::GetInst()->LoadSound("SKILL", "FireBallQShoot", false, "FireBallQShoot.mp3");
	CAssetManager::GetInst()->LoadSound("SKILL", "FireBallQExplosion", false, "FireBallQExplosion.mp3");
	CAssetManager::GetInst()->LoadSound("SKILL", "MeteorExplosion", false, "MeteorExplosion.mp3");
	CAssetManager::GetInst()->LoadSound("SKILL", "FireSword", false, "FireSword.mp3");
	
	CAssetManager::GetInst()->SetVolume("SKILL", 100);


#pragma endregion

	CParticleSystemManager::GetInst()->Init(this);
	CEffectManager::GetInst()->Init();

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
		this->AddGameObject(c, LAYER_CAMERA, false);
	}
#pragma endregion

	CGameObject* skybox = new CGameObject;
	skybox->SetName(L"Skybox");
	skybox->AddComponent(new CTransform);
	skybox->AddComponent(new CMeshRenderer);
	skybox->GetMeshRenderer()->SetMesh(CAssetManager::GetInst()->FindAsset<CMesh>(L"Cube"));
	skybox->GetMeshRenderer()->SetMaterial(CAssetManager::GetInst()->FindAsset<CMaterial>(L"Skybox"));
	this->AddGameObject(skybox, LAYER_BACKGROUND, false);


	// directional light
	{
		CGameObject* light = new CGameObject;
		light->SetName(L"DirectionalLight");
		light->AddComponent(new CTransform);
		light->AddComponent(new CLight);
		light->GetTransform()->SetRelativePosition(6000.f, 10000.f, 6000.f);
		light->GetLight()->SetLightDirection(Vec3(.3f, -1.f, -.3f));
		light->GetLight()->SetLightType(LIGHT_TYPE::DIRECTIONAL_LIGHT);
		light->GetLight()->SetDiffuse(Vec3(0.5f, 0.5f, 0.5f));
		light->GetLight()->SetAmbient(Vec3(0.7f, 0.7f, 0.7f));
		light->GetLight()->SetSpecular(Vec3(0.5f, 0.5f, 0.5f));
		light->GetLight()->GetShadowCamera()->GetCamera()->SetFar(1000000);
		CRenderManager::GetInst()->RegisterLight(light->GetLight());

		this->AddGameObject(light, LAYER_LIGHT, false);
	}

	
#ifdef DEBUG_SOLOPLAY
	CPlayer* player = new CPlayer(EPlayerAttribute::Fire, true);
	this->AddGameObject(player, 3, false);
	CLevelManager::GetInst()->SetOwnPlayer(player);
#endif // DEBUG_SOLOPLAY

	//{
	//	CMonster* monster = new CMonster();
	//	monster->SetName(L"crab1");
	//	monster->GetTransform()->SetRelativePosition(7000, -300, 3200);
	//	monster->GetTransform()->SetRelativeScale(0.5f, 0.5f, 0.5f);
	//	monster->GetTransform()->SetRelativeRotation(0.f, 270.f, 0.f);
	//	this->AddGameObject(monster, LAYER_MONSTER, false);
	//} {
	//	CMonster* monster = new CMonster();
	//	monster->SetName(L"crab2");
	//	monster->GetTransform()->SetRelativePosition(7000, -300, 4500);
	//	monster->GetTransform()->SetRelativeScale(0.5f, 0.5f, 0.5f);
	//	monster->GetTransform()->SetRelativeRotation(0.f, 270.f, 0.f);
	//	this->AddGameObject(monster, LAYER_MONSTER, false);
	//} {
	//	CMonster* monster = new CMonster();
	//	monster->SetName(L"crab3");
	//	monster->GetTransform()->SetRelativePosition(6675, -300, 4000);
	//	monster->GetTransform()->SetRelativeScale(0.5f, 0.5f, 0.5f);
	//	monster->GetTransform()->SetRelativeRotation(0.f, 270.f, 0.f);
	//	this->AddGameObject(monster, LAYER_MONSTER, false);
	//} {
	//	CMonster* monster = new CMonster();
	//	monster->SetName(L"crab4");
	//	monster->GetTransform()->SetRelativePosition(6675, -300, 3700);
	//	monster->GetTransform()->SetRelativeScale(0.5f, 0.5f, 0.5f);
	//	monster->GetTransform()->SetRelativeRotation(0.f, 270.f, 0.f);
	//	this->AddGameObject(monster, LAYER_MONSTER, false);
	//}

	//// 1번
	//{
	//	CMonster* monster = new CMonster();
	//	monster->SetName(L"crab1");
	//	monster->GetTransform()->SetRelativePosition(7000, -300 * 0.5, 3200);
	//	monster->GetTransform()->SetRelativeScale(0.5f, 0.5f, 0.5f);
	//	monster->GetTransform()->SetRelativeRotation(0.f, 270.f, 0.f);
	//	this->AddGameObject(monster, LAYER_MONSTER, false);
	//}
	//{
	//	CMonster* monster = new CMonster();
	//	monster->SetName(L"crab2");
	//	monster->GetTransform()->SetRelativePosition(7000, -300 * 0.5, 4500);
	//	monster->GetTransform()->SetRelativeScale(0.5f, 0.5f, 0.5f);
	//	monster->GetTransform()->SetRelativeRotation(0.f, 270.f, 0.f);
	//	this->AddGameObject(monster, LAYER_MONSTER, false);
	//}
	//{
	//	CMonster* monster = new CMonster();
	//	monster->SetName(L"crab3");
	//	monster->GetTransform()->SetRelativePosition(7500, -300 * 0.5, 3850.f);
	//	monster->GetTransform()->SetRelativeScale(0.5f, 0.5f, 0.5f);
	//	monster->GetTransform()->SetRelativeRotation(0.f, 270.f, 0.f);
	//	this->AddGameObject(monster, LAYER_MONSTER, false);
	//}
	//{
	//	CMonster* monster = new CMonster(EMonsterType::Adc);
	//	monster->SetName(L"adc1");
	//	monster->GetTransform()->SetRelativePosition(6300, 20, 4300);
	//	monster->GetTransform()->SetRelativeScale(0.7f, 0.7f, 0.7f);
	//	monster->GetTransform()->SetRelativeRotation(0.f, 270.f, 0.f);
	//	this->AddGameObject(monster, LAYER_MONSTER, false);
	//} {
	//	CMonster* monster = new CMonster(EMonsterType::Adc);
	//	monster->SetName(L"adc2");
	//	monster->GetTransform()->SetRelativePosition(6300, 20, 3400);
	//	monster->GetTransform()->SetRelativeScale(0.7f, 0.7f, 0.7f);
	//	monster->GetTransform()->SetRelativeRotation(0.f, 270.f, 0.f);
	//	this->AddGameObject(monster, LAYER_MONSTER, false);
	//}

	//// 2번
	//{
	//	CMonster* monster = new CMonster();
	//	monster->SetName(L"crab4");
	//	monster->GetTransform()->SetRelativePosition(4000, -300 * 0.5, 3200);
	//	monster->GetTransform()->SetRelativeScale(0.5f, 0.5f, 0.5f);
	//	monster->GetTransform()->SetRelativeRotation(0.f, 270.f, 0.f);
	//	this->AddGameObject(monster, LAYER_MONSTER, false);
	//} 
	//{
	//	CMonster* monster = new CMonster();
	//	monster->SetName(L"crab5");
	//	monster->GetTransform()->SetRelativePosition(4000, -300 * 0.5, 4500);
	//	monster->GetTransform()->SetRelativeScale(0.5f, 0.5f, 0.5f);
	//	monster->GetTransform()->SetRelativeRotation(0.f, 270.f, 0.f);
	//	this->AddGameObject(monster, LAYER_MONSTER, false);
	//} {
	//	CMonster* monster = new CMonster(EMonsterType::Adc);
	//	monster->SetName(L"adc3");
	//	monster->GetTransform()->SetRelativePosition(3275, 20, 3975);
	//	monster->GetTransform()->SetRelativeScale(0.7f, 0.7f, 0.7f);
	//	monster->GetTransform()->SetRelativeRotation(0.f, 270.f, 0.f);
	//	this->AddGameObject(monster, LAYER_MONSTER, false);
	//} {
	//	CMonster* monster = new CMonster(EMonsterType::Adc);
	//	monster->SetName(L"adc4");
	//	monster->GetTransform()->SetRelativePosition(3275, 20, 3675);
	//	monster->GetTransform()->SetRelativeScale(0.7f, 0.7f, 0.7f);
	//	monster->GetTransform()->SetRelativeRotation(0.f, 270.f, 0.f);
	//	this->AddGameObject(monster, LAYER_MONSTER, false);
	//}

	//// 3번
	//{
	//	CMonster* monster = new CMonster();
	//	monster->SetName(L"crab6");
	//	monster->GetTransform()->SetRelativePosition(0, -300 * 0.5, 3850);
	//	monster->GetTransform()->SetRelativeScale(0.5f, 0.5f, 0.5f);
	//	monster->GetTransform()->SetRelativeRotation(0.f, 270.f, 0.f);
	//	this->AddGameObject(monster, LAYER_MONSTER, false);
	//}
	//{
	//	CMonster* monster = new CMonster();
	//	monster->SetName(L"crab7");
	//	monster->GetTransform()->SetRelativePosition(-300, -300 * 0.5, 2500);
	//	monster->GetTransform()->SetRelativeScale(0.5f, 0.5f, 0.5f);
	//	monster->GetTransform()->SetRelativeRotation(0.f, 270.f, 0.f);
	//	this->AddGameObject(monster, LAYER_MONSTER, false);
	//} 
	//{
	//	CMonster* monster = new CMonster(EMonsterType::Adc);
	//	monster->SetName(L"adc5");
	//	monster->GetTransform()->SetRelativePosition(-500, 20, 3850);
	//	monster->GetTransform()->SetRelativeScale(0.7f, 0.7f, 0.7f);
	//	monster->GetTransform()->SetRelativeRotation(0.f, 270.f, 0.f);
	//	this->AddGameObject(monster, LAYER_MONSTER, false);
	//}

	//// 4번
	//{
	//	CMonster* monster = new CMonster();
	//	monster->SetName(L"crab11");
	//	monster->GetTransform()->SetRelativePosition(-800, -300 * 0.5, 8000);
	//	monster->GetTransform()->SetRelativeScale(0.5f, 0.5f, 0.5f);
	//	this->AddGameObject(monster, LAYER_MONSTER, false);
	//}
	//{
	//	CMonster* monster = new CMonster();
	//	monster->SetName(L"crab12");
	//	monster->GetTransform()->SetRelativePosition(-1800, -300 * 0.5, 8000);
	//	monster->GetTransform()->SetRelativeScale(0.5f, 0.5f, 0.5f);
	//	this->AddGameObject(monster, LAYER_MONSTER, false);
	//} {
	//	CMonster* monster = new CMonster();
	//	monster->SetName(L"crab13");
	//	monster->GetTransform()->SetRelativePosition(-1300, -300 * 0.5, 7500);
	//	monster->GetTransform()->SetRelativeScale(0.5f, 0.5f, 0.5f);
	//	this->AddGameObject(monster, LAYER_MONSTER, false);
	//} 
	//{
	//	CMonster* monster = new CMonster(EMonsterType::Adc);
	//	monster->SetName(L"adc6");
	//	monster->GetTransform()->SetRelativePosition(-650, 20, 7050);
	//	monster->GetTransform()->SetRelativeRotation(0, 90, 0);
	//	monster->GetTransform()->SetRelativeScale(0.7f, 0.7f, 0.7f);
	//	this->AddGameObject(monster, LAYER_MONSTER, false);
	//}
	//{
	//	CMonster* monster = new CMonster(EMonsterType::Adc);
	//	monster->SetName(L"adc7");
	//	monster->GetTransform()->SetRelativePosition(-650, 20, 8250);
	//	monster->GetTransform()->SetRelativeRotation(0, 45, 0);
	//	monster->GetTransform()->SetRelativeScale(0.7f, 0.7f, 0.7f);
	//	this->AddGameObject(monster, LAYER_MONSTER, false);
	//}
	//{
	//	CMonster* monster = new CMonster(EMonsterType::Adc);
	//	monster->SetName(L"adc8");
	//	monster->GetTransform()->SetRelativePosition(-2000, 20, 7050);
	//	monster->GetTransform()->SetRelativeRotation(0, -90, 0);
	//	monster->GetTransform()->SetRelativeScale(0.7f, 0.7f, 0.7f);
	//	this->AddGameObject(monster, LAYER_MONSTER, false);
	//}
	//{
	//	CMonster* monster = new CMonster(EMonsterType::Adc);
	//	monster->SetName(L"adc9");
	//	monster->GetTransform()->SetRelativePosition(-2000, 20, 8250);
	//	monster->GetTransform()->SetRelativeRotation(0, -45, 0);
	//	monster->GetTransform()->SetRelativeScale(0.7f, 0.7f, 0.7f);
	//	this->AddGameObject(monster, LAYER_MONSTER, false);
	//}

	//{
	//	CNPC* npc = new CNPC();
	//	npc->GetTransform()->SetRelativePosition(11000, 20, 3500);
	//	npc->SetWidgetWindowType(EWIDGETWINDOW_TYPE::STORE_WINDOW);
	//	this->AddGameObject(npc, LAYER_NPC, false);
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
		this->AddGameObject(o, LAYER_MAP, false);

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
	this->AddGameObject(camera, LAYER_CAMERA, false);

#pragma region UI MAP

	float centerX = 4919.f;
	float centerZ = 6754.f;
	float camHeight = 84667.f;
	float fov = 10.f;

	CGameObject* mapCamera = new CGameObject;
	mapCamera->SetName(L"MiniMapCamera");
	mapCamera->AddComponent(new CTransform);
	mapCamera->AddComponent(new CCamera);
	mapCamera->GetCamera()->SetProjType(EProjection_Type::Perspective);
	mapCamera->GetCamera()->SetFOV(fov); // 0 : main camera
	mapCamera->GetCamera()->SetPriority(2); // 0 : main camera
	mapCamera->GetCamera()->CheckLayerAll();
	mapCamera->GetCamera()->CheckLayer(4);
	mapCamera->GetCamera()->SetFar(100000);
	mapCamera->GetTransform()->SetRelativePosition(centerX, camHeight, centerZ);
	mapCamera->GetTransform()->SetRelativeRotation(90.f, 0.f, 0.f);
	this->AddGameObject(mapCamera, LAYER_CAMERA, false);

	float aspect = (float)(CEngine::GetInst()->GetWindowInfo().Width) / (float)(CEngine::GetInst()->GetWindowInfo().Height);   
	float fovrad = XMConvertToRadians(fov);
	float halfHeight = camHeight * tan(fovrad / 2.0f);
	float halfWidth = halfHeight * aspect;
	
	m_MapMax = { centerX - halfWidth, centerZ - halfHeight };
	m_MapMin = { centerX + halfWidth, centerZ + halfHeight };

#pragma endregion

#pragma region Widget

	//CreateWidgetWindow<CPlayWidgetWindow>(EWIDGETWINDOW_TYPE::GAME_WINDOW, L"GamePlayWidget");
	//const auto& win = CreateWidgetWindow<CStoreWidgetWindow>(EWIDGETWINDOW_TYPE::STORE_WINDOW, L"StoreWindow", nullptr);
	//win->SetEnable(false);
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
		this->AddGameObject(obj, LAYER_UI, false);
	}

#pragma endregion

#pragma region PORTAL_TEST
	{
		CPortal* portal = new CPortal();
		portal->GetTransform()->SetRelativePosition(Vec3(-1334.f, 0.f, 12166.f));
		this->AddGameObject(portal, LAYER_GATE, true);

	}
#pragma endregion

#ifdef AUTO_SERVER_CONNECT
	CNetworkManager::GetInst()->s_GameSession->SelectMageAttribute(m_Attribute);
#endif
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
	CEffectManager::GetInst()->Clear();
}
