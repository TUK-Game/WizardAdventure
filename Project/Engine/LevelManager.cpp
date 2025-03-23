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
#include "UI.h"
#include "UIButton.h"
#include <iostream>
#include "Animator.h"
#include "TestPlayer.h"
#include "SubLevel.h"

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
	m_CurLevel->Init();
	m_CurLevel->Begin();

	return S_OK;
}

void CLevelManager::Progress()
{
	m_CurLevel->Update();

	//m_CurLevel->Deregister();

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
		std::vector<CGameObject*> gameObjects;
		if (j == 3 || j == 10)
			m_CurLevel->m_SubLevel->PickGameObject(gameObjects, j);
		else
			gameObjects = GetCurrentLevel()->GetLayer(j)->GetParentObjects();

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
