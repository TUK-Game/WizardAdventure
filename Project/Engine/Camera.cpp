#include "pch.h"
#include "Camera.h"
#include "Engine.h"
#include "RenderManager.h"
#include "LevelManager.h"
#include "Level.h"
#include "Layer.h"
#include "Transform.h"
#include "RenderComponent.h"
#include "BaseCollider.h"
#include "Light.h"
#include "Device.h"
Matrix CCamera::s_matView;
Matrix CCamera::s_matProjection;

CCamera::CCamera()
	: CComponent(EComponent_Type::Camera)
	, m_ProjectionType(EProjection_Type::Perspective)
	, m_OrthoScaleX(1.f)
	, m_AspectRatio(1.f)
	, m_FOV(XM_PI / 2.f)
	, m_Far(10000.f)
	, m_Priority(-1)
	, m_LayerCheck(0)
{
	Vec2 resolution = CEngine::GetInst()->GetResolution();
	m_OrthoScaleX = resolution.x;
	m_AspectRatio = resolution.x / resolution.y;
}

CCamera::~CCamera()
{
}

void CCamera::FinalUpdate()
{
	// 뷰행렬 계산
	// 이동
	Vec3 vCamPos = GetTransform()->GetRelativePosition();
	Matrix matTrans = XMMatrixTranslation(-vCamPos.x, -vCamPos.y, -vCamPos.z);

	// 회전
	Vec3 vR = GetTransform()->GetWorldDir(EDir::Right);
	Vec3 vU = GetTransform()->GetWorldDir(EDir::Up);
	Vec3 vF = GetTransform()->GetWorldDir(EDir::Front);

	Matrix matRot = XMMatrixIdentity();
	matRot._11 = vR.x;	matRot._12 = vU.x;	matRot._13 = vF.x;
	matRot._21 = vR.y;	matRot._22 = vU.y;	matRot._23 = vF.y;
	matRot._31 = vR.z;	matRot._32 = vU.z;	matRot._33 = vF.z;

	m_matView = matTrans * matRot;


	// 투영행렬 계산하기
	if (m_ProjectionType == EProjection_Type::Orthographic)
	{
		m_matProjection = XMMatrixOrthographicLH(m_OrthoScaleX, m_OrthoScaleX / m_AspectRatio, 1.f, m_Far);
	}
	else
	{
		m_matProjection = XMMatrixPerspectiveFovLH(m_FOV, m_AspectRatio, 1.f, m_Far);
	}

	// 카메라의 View, Proj 행렬을 세팅
	s_matView = m_matView;
	s_matProjection = m_matProjection;

	m_Frustum.FinalUpdate();
}

void CCamera::Render()
{
	// 오브젝트 분류
	SortObject();

	PushLightData();

	// 오브젝트 렌더링
	for (auto& object : m_vecObjects)
	{
		object->Render();
	}

	m_vecObjects.clear();
}

void CCamera::SetPriority(int priority)
{
	m_Priority = priority;
	CRenderManager::GetInst()->RegisterCamera(this, priority);
}

void CCamera::SortObject()
{
	CLevel* pCurLevel = CLevelManager::GetInst()->GetCurrentLevel();

	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		// 카메라가 렌더링하지 않는 레이어는 무시한다.
		if (!(m_LayerCheck & (1 << i)))
			continue;

		// 레이어에 속한 오브젝트를 가져온다.
		CLayer* pLayer = pCurLevel->GetLayer(i);
		const std::vector<CGameObject*>& vecObjects = pLayer->GetObjects();

		for (size_t j = 0; j < vecObjects.size(); ++j)
		{
			// 프러스텀 컬링
			if (vecObjects[j]->GetCheckFrustum() && vecObjects[j]->GetCollider())
			{
				if (!vecObjects[j]->GetCollider()->IsFrustum(m_Frustum))
				{
					continue;
				}
			}

			// 레이어 안에있는 물체들 중에서 렌더링 기능이 없는 물체는 거른다.
			// TODO: Material 구현시 예외처리 추가
			if ((vecObjects[j]->GetRenderComponent() == nullptr
				|| vecObjects[j]->GetRenderComponent()->GetMesh() == nullptr) && vecObjects[j]->GetLight() == nullptr)
				continue;

			m_vecObjects.push_back(vecObjects[j]);

			// TODO: Material 구현시 타입에 따른 분류 작성
		}
	}
}

void CCamera::PushLightData()
{
	LightParams lightParams = {};

	for (auto& vecObject : m_vecObjects)
	{
		if (vecObject->GetLight() == nullptr)
			continue;

		const LightInfo& lightInfo = vecObject->GetLight()->GetLightInfo();
		lightParams.lights[lightParams.lightCount] = lightInfo;
		++lightParams.lightCount;
	}

	CONST_BUFFER(EConstantBuffer_Type::Global)->SetGlobalData(&lightParams, sizeof(lightParams));
}