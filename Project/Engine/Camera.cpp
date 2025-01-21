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
	// ����� ���
	// �̵�
	Vec3 vCamPos = GetTransform()->GetRelativePosition();
	Matrix matTrans = XMMatrixTranslation(-vCamPos.x, -vCamPos.y, -vCamPos.z);

	// ȸ��
	Vec3 vR = GetTransform()->GetWorldDir(EDir::Right);
	Vec3 vU = GetTransform()->GetWorldDir(EDir::Up);
	Vec3 vF = GetTransform()->GetWorldDir(EDir::Front);

	Matrix matRot = XMMatrixIdentity();
	matRot._11 = vR.x;	matRot._12 = vU.x;	matRot._13 = vF.x;
	matRot._21 = vR.y;	matRot._22 = vU.y;	matRot._23 = vF.y;
	matRot._31 = vR.z;	matRot._32 = vU.z;	matRot._33 = vF.z;

	m_matView = matTrans * matRot;


	// ������� ����ϱ�
	if (m_ProjectionType == EProjection_Type::Orthographic)
	{
		m_matProjection = XMMatrixOrthographicLH(m_OrthoScaleX, m_OrthoScaleX / m_AspectRatio, 1.f, m_Far);
	}
	else
	{
		m_matProjection = XMMatrixPerspectiveFovLH(m_FOV, m_AspectRatio, 1.f, m_Far);
	}

	// ī�޶��� View, Proj ����� ����
	s_matView = m_matView;
	s_matProjection = m_matProjection;

	m_Frustum.FinalUpdate();
}

void CCamera::Render()
{
	// ������Ʈ �з�
	SortObject();

	PushLightData();

	// ������Ʈ ������
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
		// ī�޶� ���������� �ʴ� ���̾�� �����Ѵ�.
		if (!(m_LayerCheck & (1 << i)))
			continue;

		// ���̾ ���� ������Ʈ�� �����´�.
		CLayer* pLayer = pCurLevel->GetLayer(i);
		const std::vector<CGameObject*>& vecObjects = pLayer->GetObjects();

		for (size_t j = 0; j < vecObjects.size(); ++j)
		{
			// �������� �ø�
			if (vecObjects[j]->GetCheckFrustum() && vecObjects[j]->GetCollider())
			{
				if (!vecObjects[j]->GetCollider()->IsFrustum(m_Frustum))
				{
					continue;
				}
			}

			// ���̾� �ȿ��ִ� ��ü�� �߿��� ������ ����� ���� ��ü�� �Ÿ���.
			// TODO: Material ������ ����ó�� �߰�
			if ((vecObjects[j]->GetRenderComponent() == nullptr
				|| vecObjects[j]->GetRenderComponent()->GetMesh() == nullptr) && vecObjects[j]->GetLight() == nullptr)
				continue;

			m_vecObjects.push_back(vecObjects[j]);

			// TODO: Material ������ Ÿ�Կ� ���� �з� �ۼ�
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