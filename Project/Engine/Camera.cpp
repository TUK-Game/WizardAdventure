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

Matrix CCamera::s_matView;
Matrix CCamera::s_matProjection;

CCamera::CCamera()
	: CComponent(EComponent_Type::Camera)
	, m_ProjectionType(EProjection_Type::Perspective)
	, m_OrthoScaleX(1.f)
	, m_AspectRatio(1.f)
	, m_FOV(XM_PI / 2.f)
	, m_Far(2000.f)
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

void CCamera::Render()
{
	s_matView = m_matView;
	s_matProjection = m_matProjection;

	SortObject();

	PushLightData();

	// Deferred
	CDevice::GetInst()->GetRenderTargetGroup(RENDER_TARGET_GROUP_TYPE::G_BUFFER)->OMSetRenderTargets();
	for (auto& object : m_vecDeferred)
	{
		object->Render();
	}

	// Light OMSet

	// Swapchain
	INT8 backIndex = CDevice::GetInst()->GetSwapChain()->GetBackBufferIndex();
	CDevice::GetInst()->GetRenderTargetGroup(RENDER_TARGET_GROUP_TYPE::SWAP_CHAIN)->OMSetRenderTargets(1, backIndex);
	for (auto& object : m_vecForward)
	{
		object->Render();
	}
}


void CCamera::Render_Deferred()
{
	s_matView = m_matView;
	s_matProjection = m_matProjection;

	CInstancingManager::GetInst()->Render(m_vecDeferred);
}

void CCamera::Render_Forward()
{
	s_matView = m_matView;
	s_matProjection = m_matProjection;

	CInstancingManager::GetInst()->Render(m_vecForward);

	for (auto& object : m_vecParticle)
	{
		object->GetParticleSystem()->Render();
	}
}

void CCamera::Render_Shadow()
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
		// ī�޶� ���������� �ʴ� ���̾�� �����Ѵ�.
		if (!(m_LayerCheck & (1 << i)))
			continue;

		// ���̾ ���� ������Ʈ�� �����´�.
		CLayer* pLayer = pCurLevel->GetLayer(i);
		const std::vector<CGameObject*>& vecObjects = pLayer->GetObjects();

		for (size_t j = 0; j < vecObjects.size(); ++j)
		{
			// ���̾� �ȿ��ִ� ��ü�� �߿��� ������ ����� ���� ��ü�� �Ÿ���.
			// TODO: Material ������ ����ó�� �߰�
			if ((vecObjects[j]->GetRenderComponent() == nullptr
				|| vecObjects[j]->GetRenderComponent()->GetMesh() == nullptr)
				&& vecObjects[j]->GetParticleSystem() == nullptr)
				continue;

			// �������� �ø�
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

			// TODO: Material ������ Ÿ�Կ� ���� �з� �ۼ�
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
		const std::vector<CGameObject*>& vecObjects = pLayer->GetObjects();

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
