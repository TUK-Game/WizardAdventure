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
#include "BoxCollider.h"
#include "OrientedBoxCollider.h"
#include "Engine.h"
#include "RenderManager.h"

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
	CGameObject* object2 = new CGameObject;
	CGameObject* object3 = new CGameObject;
	CGameObject* object4 = new CGameObject;

	object4->SetName(L"Cube4");
	object4->AddComponent(new CTransform);
	object4->AddComponent(new CMeshRenderer);
	object4->AddComponent(new COrientedBoxCollider);
	object4->GetTransform()->SetRelativeScale(100.f, 100.f, 100.f);
	object4->GetTransform()->SetRelativeRotation(0.f, 0.f, 0.f);
	object4->GetTransform()->SetRelativePosition(100.f, 0.f, 0.f);
	object4->GetMeshRenderer()->SetMesh(CAssetManager::GetInst()->FindAsset<CMesh>(L"Cube"));
	object4->GetMeshRenderer()->SetMaterial(CAssetManager::GetInst()->FindAsset<CMaterial>(L"Kita"));

	object3->SetName(L"Cube3");
	object3->AddComponent(new CTransform);
	object3->AddComponent(new CMeshRenderer);
	object3->AddComponent(new COrientedBoxCollider);
	object3->GetTransform()->SetRelativeScale(	100.f, 100.f, 100.f);
	object3->GetTransform()->SetRelativeRotation(0.f, 0.f, 0.f);
	object3->GetTransform()->SetRelativePosition(300.f, 0.f, 0.f);
	object3->GetMeshRenderer()->SetMesh(CAssetManager::GetInst()->FindAsset<CMesh>(L"Cube"));
	object3->GetMeshRenderer()->SetMaterial(CAssetManager::GetInst()->FindAsset<CMaterial>(L"Ryo"));
	object3->AddChild(object4);

	object2->SetName(L"Cube2");
	object2->AddComponent(new CTransform);
	object2->AddComponent(new CMeshRenderer);
	object2->AddComponent(new COrientedBoxCollider);
	object2->GetTransform()->SetRelativeScale(100.f, 100.f, 100.f);
	object2->GetTransform()->SetRelativeRotation(0.f, 0.f, 0.f);
	object2->GetTransform()->SetRelativePosition(200.f, 0.f, 0.f);
	object2->GetMeshRenderer()->SetMesh(CAssetManager::GetInst()->FindAsset<CMesh>(L"Cube"));
	object2->GetMeshRenderer()->SetMaterial(CAssetManager::GetInst()->FindAsset<CMaterial>(L"Nigika"));

	object->SetName(L"Cube");
	object->AddComponent(new CTransform);
	object->AddComponent(new CMeshRenderer);
	object->AddComponent(new COrientedBoxCollider);
	object->GetTransform()->SetRelativeScale(100.f, 100.f, 100.f);
	object->GetTransform()->SetRelativeRotation(0.f, 0.f, 0.f);
	object->GetTransform()->SetRelativePosition(-300.f, 0.f, 300.f);
	object->GetMeshRenderer()->SetMesh(CAssetManager::GetInst()->FindAsset<CMesh>(L"Cube"));
	object->GetMeshRenderer()->SetMaterial(CAssetManager::GetInst()->FindAsset<CMaterial>(L"Hitori"));
	object->AddChild(object2);
	object->AddChild(object3);
	m_CurLevel->AddGameObject(object, 3, false);

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

	for (int j = 0; j < MAX_LAYER; ++j)
	{
		auto& gameObjects = GetCurrentLevel()->GetLayer(j)->GetObjects();
		for (auto& gameObject : gameObjects)
		{
			if (gameObject->GetCollider() == nullptr)
				continue;

			// ViewSpace에서의 Ray 정의
			Vec4 rayOrigin = Vec4(0.0f, 0.0f, 0.0f, 1.0f);
			Vec4 rayDir = Vec4(viewX, viewY, 1.0f, 0.0f);

			// WorldSpace에서의 Ray 정의
			rayOrigin = XMVector3TransformCoord(rayOrigin, viewMatrixInv);
			rayDir = XMVector3TransformNormal(rayDir, viewMatrixInv);
			rayDir.Normalize();

			// WorldSpace에서 연산
			float distance = 0.f;
			if (gameObject->GetCollider()->Intersects(rayOrigin, rayDir, OUT distance) == false)
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
