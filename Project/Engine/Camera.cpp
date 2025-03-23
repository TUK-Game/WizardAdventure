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
#include "MeshRenderer.h"
#include "Device.h"
#include "ParticleSystem.h"
#include "InstancingManager.h"
#include "AssetManager.h"

#include <iostream>
#include "SubLevel.h"

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
	m_Width = static_cast<float>(CEngine::GetInst()->GetWindowInfo().Width);
	m_Height = static_cast<float>(CEngine::GetInst()->GetWindowInfo().Height);
}

CCamera::~CCamera()
{
}

void CCamera::FinalUpdate()
{
	if (m_bUseCustomMatView)  // 수동으로 조작된 경우
	{
		m_matView = m_CustomMatView;  // 저장된 사용자 뷰 행렬 적용
		m_bUseCustomMatView = false;
	}
	else
	{
		Vec3 vCamPos = GetTransform()->GetRelativePosition();
		Matrix matTrans = XMMatrixTranslation(-vCamPos.x, -vCamPos.y, -vCamPos.z);

		Vec3 vR = GetTransform()->GetWorldDir(EDir::Right);
		Vec3 vU = GetTransform()->GetWorldDir(EDir::Up);
		Vec3 vF = GetTransform()->GetWorldDir(EDir::Front);

		Matrix matRot = XMMatrixIdentity();
		matRot._11 = vR.x;	matRot._12 = vU.x;	matRot._13 = vF.x;
		matRot._21 = vR.y;	matRot._22 = vU.y;	matRot._23 = vF.y;
		matRot._31 = vR.z;	matRot._32 = vU.z;	matRot._33 = vF.z;

		m_matView = matTrans * matRot; 
	}



	if (m_ProjectionType == EProjection_Type::Orthographic)
	{
		m_matProjection = ::XMMatrixOrthographicLH(m_Width * m_Scale, m_Height * m_Scale, m_Near, m_Far);
	}
	else
	{
		m_matProjection = ::XMMatrixPerspectiveFovLH(m_FOV, m_Width / m_Height, m_Near, m_Far);
	}


	s_matView = m_matView;
	s_matProjection = m_matProjection;

	m_Frustum.FinalUpdate();
}

void CCamera::RenderMap(CGameObject* object)
{
	s_matView = m_matView;
	s_matProjection = m_matProjection;


	object->AddComponent(new CTransform);
	object->AddComponent(new CMeshRenderer);
	object->GetMeshRenderer()->SetMaterial(CAssetManager::GetInst()->FindAsset<CMaterial>(L"Map"));
	object->GetMeshRenderer()->SetMesh(CAssetManager::GetInst()->FindAsset<CMesh>(L"Cube"));
	object->GetTransform()->SetRelativePosition(5000, -100, 6500);
	object->GetTransform()->SetRelativeScale(27000, 1, 16000);
	object->GetTransform()->FinalUpdate();
	object->GetMeshRenderer()->RenderMap();


	for (auto& object : m_vecDeferred)
	{
		object->GetMeshRenderer()->RenderMap();
	}
}


void CCamera::RenderDeferred()
{
	s_matView = m_matView;
	s_matProjection = m_matProjection;

	CInstancingManager::GetInst()->Render(m_vecDeferred, L"Deferred");
}

void CCamera::RenderForward()
{
	s_matView = m_matView;
	s_matProjection = m_matProjection;

	CInstancingManager::GetInst()->Render(m_vecForward, L"Forward");

	for (auto& object : m_vecParticle)
	{
		object->GetParticleSystem()->Render();
	}
}

void CCamera::RenderShadow()
{
	s_matView = m_matView;
	m_matProjection = m_matProjection;

	for (auto& object : m_vecShadow)
	{
		object->GetMeshRenderer()->RenderShadow();
	}
}

void CCamera::SetPriority(int priority)
{
	m_Priority = priority;
	CRenderManager::GetInst()->RegisterCamera(this, priority);
}

void CCamera::SortObject()
{
	m_vecForward.clear();
	m_vecDeferred.clear();
	m_vecParticle.clear();

	CLevel* pCurLevel = CLevelManager::GetInst()->GetCurrentLevel();
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		if (!(m_LayerCheck & (1 << i)))
			continue;

		std::vector<CGameObject*> vecObjects;
		if (i == 3 || i == 10)
		{
			std::shared_ptr<CSubLevel> level = pCurLevel->m_SubLevel;
			if (level)
			{
				if (m_Frustum.IsInFrustum(pCurLevel->m_SubLevel->GetBoundingBox()))
				{
					level->PickGameObject(m_Frustum, vecObjects, i);
				}
			}
		}
		else
		{
			vecObjects = pCurLevel->GetLayer(i)->GetParentObjects();
		}

		for (size_t j = 0; j < vecObjects.size(); ++j)
		{
			if ((vecObjects[j]->GetRenderComponent() == nullptr
				|| vecObjects[j]->GetRenderComponent()->GetMesh() == nullptr)
				&& vecObjects[j]->GetParticleSystem() == nullptr)
				continue;

			if (vecObjects[j]->GetCheckFrustum() && vecObjects[j]->GetCollider())
			{
				if (!vecObjects[j]->GetCollider()->IsFrustum(m_Frustum))
				{
					continue;
				}
			}

			if (vecObjects[j]->GetMeshRenderer())
			{
				SHADER_TYPE shaderType = vecObjects[j]->GetMeshRenderer()->GetMaterial()->GetGraphicsShader()->GetShaderType();
				switch (shaderType)
				{
				case SHADER_TYPE::DEFERRED:
					m_vecDeferred.push_back(vecObjects[j]);
					break;
				case SHADER_TYPE::FORWARD:
					m_vecForward.push_back(vecObjects[j]);
					break;
				}
			}
			else
			{
				m_vecParticle.push_back(vecObjects[j]);
			}
			//m_vecObjects.push_back(vecObjects[j]);
		}
	}
}

void CCamera::SortShadowObject()
{
	m_vecShadow.clear();

	CLevel* pCurLevel = CLevelManager::GetInst()->GetCurrentLevel();

	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		if (!(m_LayerCheck & (1 << i)))
			continue;

		CLayer* pLayer = pCurLevel->GetLayer(i);
		const std::vector<CGameObject*>& vecObjects = pLayer->GetParentObjects();

		for (size_t j = 0; j < vecObjects.size(); ++j)
		{
			if ((vecObjects[j]->GetRenderComponent() == nullptr
				|| vecObjects[j]->GetRenderComponent()->GetMesh() == nullptr)
				&& vecObjects[j]->GetParticleSystem() == nullptr)
				continue;

			if (vecObjects[j]->GetCheckFrustum() && vecObjects[j]->GetCollider())
			{
				if (!vecObjects[j]->GetCollider()->IsFrustum(m_Frustum))
				{
					continue;
				}
			}

			if (vecObjects[j]->GetMeshRenderer() == nullptr)
			{
				continue;
			}
				
			if (vecObjects[j]->IsStatic())
				continue;

			m_vecShadow.push_back(vecObjects[j]);
		} 
	}
	std::wstring name = this->GetName();
	int a = 3;
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


void CCamera::SetCustomMatView(const Matrix& matView)
{

	m_CustomMatView = matView;

	Matrix InvertMatView = matView.Invert();
	GetTransform()->SetRelativePosition(InvertMatView._41, InvertMatView._42, InvertMatView._43);

	// 카메라의 회전 방향 설정 (월드 방향 벡터 추출)
	Vec3 right(matView._11, matView._12, matView._13);
	Vec3 up(matView._21, matView._22, matView._23);
	Vec3 forward(matView._31, matView._32, matView._33);

	// 회전 행렬 생성
	Matrix rotationMatrix = XMMatrixIdentity();
	rotationMatrix._11 = right.x;  rotationMatrix._12 = up.x;  rotationMatrix._13 = forward.x;
	rotationMatrix._21 = right.y;  rotationMatrix._22 = up.y;  rotationMatrix._23 = forward.y;
	rotationMatrix._31 = right.z;  rotationMatrix._32 = up.z;  rotationMatrix._33 = forward.z;

	// 회전 행렬을 쿼터니언으로 변환 후 카메라 회전값 적용
	Quaternion quatRotation = XMQuaternionRotationMatrix(rotationMatrix);
	GetTransform()->SetRelativeRotation(quatRotation);

	Vec3 pos = GetTransform()->GetRelativePosition();
	std::cout << "camera pos: " << pos.x << " " << pos.y << " " << pos.z << std::endl;
	m_bUseCustomMatView = true;  // 커스텀 뷰 행렬 사용
}