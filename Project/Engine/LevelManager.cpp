#include "pch.h"
#include "LevelManager.h"
#include "AssetManager.h"
#include "Level.h"
#include "Layer.h"
#include "GameObject.h"
#include "Camera.h"
#include "Transform.h"
#include "Light.h"
#include "MeshRenderer.h"
#include "CameraScript.h"
#include "BoxCollider.h"
#include "OrientedBoxCollider.h"
#include "Engine.h"
#include "RenderManager.h"
#include "CollisionManager.h"
#include "MeshData.h"
#include "Device.h"
#include "ComputeShader.h"
#include "ParticleSystem.h"
#include "RigidBody.h"
#include "TestDragon.h"
#include "UI.h"
#include "UIButton.h"
#include <iostream>
#include "Animator.h"
#include "TestPlayer.h"

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
	m_CurLevel->GetLayer(0)->SetName(L"Camera");
	m_CurLevel->GetLayer(1)->SetName(L"BackGround");
	m_CurLevel->GetLayer(2)->SetName(L"Other");
	m_CurLevel->GetLayer(3)->SetName(L"Others");
	m_CurLevel->GetLayer(4)->SetName(L"UI");
	m_CurLevel->GetLayer(10)->SetName(L"Map");

	CAssetManager::GetInst()->LoadSound("BGM", "Play", false, "play.mp3");
	CAssetManager::GetInst()->SetVolume("BGM", 30);
	CAssetManager::GetInst()->SoundPlay("Play");

	CAssetManager::GetInst()->LoadSound("UI", "Click", false, "click.mp3");
	CAssetManager::GetInst()->SetVolume("UI", 30);

#pragma region ComputeShader
	{
		// UAV 용 Texture 생성
		CTexture* texture = CAssetManager::GetInst()->CreateTexture(L"UAVTexture",
			DXGI_FORMAT_R8G8B8A8_UNORM, 1024, 1024,
			CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT), D3D12_HEAP_FLAG_NONE,
			D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS);

		//CMaterial* material = GET_SINGLE(Resources)->Get<CMaterial>(L"ComputeShader");
		CMaterial* material = CAssetManager::GetInst()->FindAsset<CMaterial>(L"Compute");
		material->SetInt(0, 1);
		CDevice::GetInst()->GetComputeDescHeap()->SetUAV(texture->GetUAVCpuHandle(), UAV_REGISTER::u0);

		// 쓰레드 그룹 (1 * 1024 * 1)
		material->Dispatch(1, 1024, 1);
	}
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
	m_CurLevel->AddGameObject(camera, 0, false);

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
		m_CurLevel->AddGameObject(c, 0, false);
	}
#pragma endregion

	CGameObject* skybox = new CGameObject;
	skybox->SetName(L"Skybox");
	skybox->AddComponent(new CTransform);
	skybox->AddComponent(new CMeshRenderer);
	skybox->GetMeshRenderer()->SetMesh(CAssetManager::GetInst()->FindAsset<CMesh>(L"Cube"));
	skybox->GetMeshRenderer()->SetMaterial(CAssetManager::GetInst()->FindAsset<CMaterial>(L"Skybox"));
	m_CurLevel->AddGameObject(skybox, 1, false);

	
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
	
		m_CurLevel->AddGameObject(light, 3, false);
	}
	// point light
	{
		//CGameObject* light = new CGameObject;
		//light->AddComponent(new CTransform);
		//light->GetTransform()->SetRelativePosition(300.f, 0.f, 0.f);
		//light->AddComponent(new CLight);
		//light->GetLight()->SetLightType(LIGHT_TYPE::POINT_LIGHT);
		//light->GetLight()->SetDiffuse(Vec3(1.0f, 1.0f, 1.0f));
		//light->GetLight()->SetAmbient(Vec3(1.0f, 1.0f, 1.0f));
		//light->GetLight()->SetSpecular(Vec3(1.0f, 1.0f, 1.0f));
		//light->GetLight()->SetLightRange(500.f);
		////light->GetLight()->SetLightAngle(XM_PI / 4);
		//CRenderManager::GetInst()->RegisterLight(light->GetLight());

		//m_CurLevel->AddGameObject(light, 3, false);
	}
	// spot light
	{
		//CGameObject* light = new CGameObject;
		//light->AddComponent(new CTransform);
		//light->GetTransform()->SetRelativePosition(300.f, 0.f, 0.f);
		////light->GetTransform()->SetRelativePosition(-500.f, 0.f, 200.f);
		//light->AddComponent(new CLight);
		//light->GetLight()->SetLightDirection(Vec3(1.f, 0.f, 0.f));
		//light->GetLight()->SetLightType(LIGHT_TYPE::POINT_LIGHT);
		//light->GetLight()->SetDiffuse(Vec3(0.0f, 0.f, 0.5f));
		//light->GetLight()->SetAmbient(Vec3(1.0f, 0.0f, 0.0f));
		//light->GetLight()->SetSpecular(Vec3(0.0f, 0.0f, 0.1f));
		//light->GetLight()->SetLightRange(500.f);
		//light->GetLight()->SetLightAngle(XM_PI / 4);
		//CRenderManager::GetInst()->RegisterLight(light->GetLight());

		//m_CurLevel->AddGameObject(light, 3, false);
	}

	//CGameObject* object = new CGameObject;
	//CGameObject* object2 = new CGameObject;
	//CGameObject* object3 = new CGameObject;
	//CGameObject* object4 = new CGameObject;
	//
	//object4->SetName(L"Cube4");
	//object4->AddComponent(new CTransform);
	//object4->AddComponent(new CMeshRenderer);
	//object4->AddComponent(new CBoxCollider);
	//
	//object4->GetCollider()->SetProfile(CCollisionManager::GetInst()->FindProfile("Default"));
	//object4->GetTransform()->SetRelativeScale(10.f, 10.f, 10.f) ;
	//object4->GetTransform()->SetRelativeRotation(0.f, 0.f, 0.f);
	//object4->GetTransform()->SetRelativePosition(0.f, 100.f, 0.f);
	//object4->GetMeshRenderer()->SetMesh(CAssetManager::GetInst()->FindAsset<CMesh>(L"Cube"));
	//object4->GetMeshRenderer()->SetMaterial(CAssetManager::GetInst()->FindAsset<CMaterial>(L"Rock"));
	//
	//object3->SetName(L"Cube3");
	//object3->AddComponent(new CTransform);
	//object3->AddComponent(new CMeshRenderer);
	//object3->AddComponent(new CBoxCollider);
	//object3->GetCollider()->SetProfile(CCollisionManager::GetInst()->FindProfile("Default"));
	//object3->GetTransform()->SetRelativeScale(100.f, 100.f, 100.f);
	//object3->GetTransform()->SetRelativeRotation(0.f, 0.f, 0.f);
	//object3->GetTransform()->SetRelativePosition(500.f, 0.f, 0.f);
	//object3->GetMeshRenderer()->SetMesh(CAssetManager::GetInst()->FindAsset<CMesh>(L"Cube"));
	//object3->GetMeshRenderer()->SetMaterial(CAssetManager::GetInst()->FindAsset<CMaterial>(L"Ryo"));
	//object3->AddChild(object4);
	//
	//object2->SetName(L"Cube2");
	//object2->AddComponent(new CTransform);
	//object2->AddComponent(new CMeshRenderer);
	//object2->AddComponent(new CBoxCollider);
	//object2->GetCollider()->SetProfile(CCollisionManager::GetInst()->FindProfile("Default"));
	//object2->GetTransform()->SetRelativeScale(100.f, 100.f, 100.f);
	//object2->GetTransform()->SetRelativeRotation(0.f, 0.f, 0.f);
	//object2->GetTransform()->SetRelativePosition(-700.f, 0.f, 300.f);
	//object2->GetMeshRenderer()->SetMesh(CAssetManager::GetInst()->FindAsset<CMesh>(L"Cube"));
	//object2->GetMeshRenderer()->SetMaterial(CAssetManager::GetInst()->FindAsset<CMaterial>(L"Nigika"));
	//m_CurLevel->AddGameObject(object2, 3, false);
	//
	//object->SetName(L"Cube");
	//object->AddComponent(new CTransform);
	//object->AddComponent(new CMeshRenderer);
	//object->AddComponent(new CBoxCollider);
	//object->GetCollider()->SetProfile(CCollisionManager::GetInst()->FindProfile("Default"));
	//object->GetTransform()->SetRelativeScale(500.f, 500.f, 500.f);
	//object->GetTransform()->SetRelativeRotation(0.f, 0.f, 0.f);
	//object->GetTransform()->SetRelativePosition(-300.f, 0.f, 300.f);
	//object->GetMeshRenderer()->SetMesh(CAssetManager::GetInst()->FindAsset<CMesh>(L"Cube"));
	//object->GetMeshRenderer()->SetMaterial(CAssetManager::GetInst()->FindAsset<CMaterial>(L"Kita"));
	//m_CurLevel->AddGameObject(object, 3, false);

#pragma region INSTANCING
	//for (INT32 i = 0; i < 50; ++i)
	//{
	//	CGameObject* obj = new CGameObject;
	//	obj->AddComponent(new CTransform);
	//	obj->AddComponent(new CMeshRenderer);
	//	obj->GetTransform()->SetRelativeScale(Vec3(25.f, 25.f, 25.f));
	//	obj->GetTransform()->SetRelativePosition(Vec3(-300.f + i * 10.f, 0.f, 100.f));
	//	obj->GetMeshRenderer()->SetMesh(CAssetManager::GetInst()->FindAsset<CMesh>(L"Sphere"));
	//	obj->GetMeshRenderer()->SetMaterial(CAssetManager::GetInst()->FindAsset<CMaterial>(L"Hitori"));
	//	// Int_0이 1이면 인스턴싱
	//	obj->GetMeshRenderer()->GetMaterial()->SetInt(0, 1);
	//	m_CurLevel->AddGameObject(obj, 3, false);
	//}
#pragma endregion
	CMeshData* data = CAssetManager::GetInst()->FindAsset<CMeshData>(L"door");
	std::vector<CGameObject*> obj = data->Instantiate();
	
	for(auto& o : obj)
	{
		std::string name = "Dragon";
		o->SetName(s2ws(name));

		//o->GetTransform()->SetRelativeScale(0.5f, 0.5f, 0.5f);
		Vec3 rot = o->GetTransform()->GetRelativeRotation();
		//rot.x += -90;	
		o->GetTransform()->SetRelativeRotation(rot);
		//o->GetTransform()->SetRelativePosition(100, 0, 0);
		//o->GetTransform()->SetRelativeScale(0.1, 0.1, 0.1);
		//o->AddComponent(new CTestPlayer);
		//o->GetMeshRenderer()->GetMaterial()->SetInt(0, 1);
		o->SetCheckFrustum(false);
		m_CurLevel->AddGameObject(o, 10, false);

		//Vec3 trans = o->GetCollider()->center;
		//Vec3 size = o->GetCollider()->size;
		//CGameObject* object = new CGameObject;
		//object->SetName(L"k");
		//object->AddComponent(new CTransform);
		//object->AddComponent(new CMeshRenderer);
		//rot = object->GetTransform()->GetRelativeRotation();
		////rot.x += -90;
		//object->GetTransform()->SetRelativeRotation(rot);
		//object->GetTransform()->SetRelativePosition(trans);	
		//object->GetTransform()->SetRelativeScale(size);
		//object->GetMeshRenderer()->SetMesh(CAssetManager::GetInst()->FindAsset<CMesh>(L"Cube"));
		//object->GetMeshRenderer()->SetMaterial(CAssetManager::GetInst()->FindAsset<CMaterial>(L"Kita"));
		//m_CurLevel->AddGameObject(object, 3, false);
	}

	//for(auto& o : obj2)
	//{
	//	std::string name = "Wolf";
	//	o->SetName(s2ws(name));
	//	o->AddComponent(new CBoxCollider);
	//	o->GetCollider()->SetProfile(CCollisionManager::GetInst()->FindProfile("Default"));
	//	o->GetTransform()->SetRelativePosition(200, 0, 0);
	//	o->GetTransform()->SetRelativeScale(100, 100, 100);
	//	CGraphicShader* shader = CAssetManager::GetInst()->FindAsset<CGraphicShader>(L"Deferred");
	//	o->GetMeshRenderer()->GetMaterial(0)->SetGraphicsShader(shader);
	//	m_CurLevel->AddGameObject(o, 3, false);
	//}

#pragma region UI_TEST
	for (INT32 i = 0; i < 6; ++i)
	{
		CGameObject* obj = new CGameObject;
		obj->AddComponent(new CTransform);
		obj->AddComponent(new CMeshRenderer);
		obj->AddComponent(new CUIButton);
		//obj->GetTransform()->SetRelativeScale(Vec3(160.f, 160.f, 160.f));
		//obj->GetTransform()->SetRelativePosition(Vec3(-500.f + (i * 200), 250.f, 500.f));
		obj->GetTransform()->SetRelativeScale(Vec3(.2f, .35f, .2f));
		obj->GetTransform()->SetRelativePosition(Vec3(-0.8f + i * 0.33f, 0.5f, 1.0f));
		obj->GetMeshRenderer()->SetMesh(CAssetManager::GetInst()->FindAsset<CMesh>(L"Rectangle"));
		CMaterial* material = new CMaterial;	
		CTexture* texture;
	
		if (i < 3)
			texture = CDevice::GetInst()->GetRenderTargetGroup(RENDER_TARGET_GROUP_TYPE::G_BUFFER)->GetRTTexture(i);
		else if (i < 5)
			texture = CDevice::GetInst()->GetRenderTargetGroup(RENDER_TARGET_GROUP_TYPE::LIGHTING)->GetRTTexture(i - 3);
		else
			texture = CDevice::GetInst()->GetRenderTargetGroup(RENDER_TARGET_GROUP_TYPE::SHADOW)->GetRTTexture(0);
			//texture = CAssetManager::GetInst()->FindAsset<CTexture>(L"UAVTexture");
	
	
		CGraphicShader* shader = CAssetManager::GetInst()->FindAsset<CGraphicShader>(L"Texture");
		material->SetTexture(0, texture);
		material->SetGraphicsShader(shader);
		obj->GetMeshRenderer()->SetMaterial(material);
		m_CurLevel->AddGameObject(obj, 4, false);
	}

#pragma endregion

#pragma region ParticleSystem
	{
		//CGameObject* particle = new CGameObject;
		//particle->AddComponent(new CTransform);
		//particle->AddComponent(new CParticleSystem);
		//particle->GetParticleSystem()->SetTexture(L"Nigika");
		//particle->SetCheckFrustum(false);		
		//particle->GetTransform()->SetRelativePosition(Vec3(0.f, 0.f, 100.f));
		//m_CurLevel->AddGameObject(particle, 3, false);
	}
#pragma endregion

	m_CurLevel->Begin();

	return S_OK;
}

void CLevelManager::Progress()
{
	m_CurLevel->Update();

	m_CurLevel->Deregister();

	m_CurLevel->FinalUpdate();
}

CGameObject* CLevelManager::Pick(INT32 x, INT32 y)
{
	CCamera* camera = CRenderManager::GetInst()->GetMainCamera();

	float width = static_cast<float>(CEngine::GetInst()->GetWindowInfo().Width);
	float height = static_cast<float>(CEngine::GetInst()->GetWindowInfo().Height);

	Matrix projectionMatrix = camera->GetProjMat();

	// ViewSpace에서 Picking 진행
	float viewX = (+2.0f * x / width - 1.0f) / projectionMatrix(0, 0);
	float viewY = (-2.0f * y / height + 1.0f) / projectionMatrix(1, 1);

	Matrix viewMatrix = camera->GetViewMat();
	Matrix viewMatrixInv = viewMatrix.Invert();

	float minDistance = FLT_MAX;
	CGameObject* picked = nullptr;

	// ViewSpace에서의 Ray 정의
	Vec4 rayOrigin = Vec4(0.0f, 0.0f, 0.0f, 1.0f);
	Vec4 rayDir = Vec4(viewX, viewY, 1.0f, 0.0f);

	// WorldSpace에서의 Ray 정의
	rayOrigin = XMVector3TransformCoord(rayOrigin, viewMatrixInv);
	rayDir = XMVector3TransformNormal(rayDir, viewMatrixInv);
	rayDir.Normalize();

	for (int j = 0; j < MAX_LAYER; ++j)
	{
		auto& gameObjects = GetCurrentLevel()->GetLayer(j)->GetObjects();
		for (auto& gameObject : gameObjects)
		{
			if (gameObject->GetCollider() == nullptr)
				continue;

			// WorldSpace에서 연산
			float distance = 0.f;
			if (!gameObject->GetCollider()->Intersects(rayOrigin, rayDir, OUT distance))
				continue;

			if (distance < minDistance)
			{
				minDistance = distance;
				picked = gameObject;
			}
		}
	}

	return picked;
}

void CLevelManager::CleanUpCurrentLevel()
{
	if (m_CurLevel)
	{
		m_CurLevel->End(); // 현재 레벨 종료 작업
		delete m_CurLevel; // 메모리 해제
		m_CurLevel = nullptr;
	}
}

void CLevelManager::ChangeLevel(CLevel* newLevel)
{
	CleanUpCurrentLevel();
	// m_CurLevel을 newLevel로 
}
