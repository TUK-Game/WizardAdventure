#include "pch.h"
#include "Light.h"
#include "Transform.h"
#include "Camera.h"
#include "AssetManager.h"
#include "RenderManager.h"
#include "Device.h"

CLight::CLight() : CComponent(EComponent_Type::Light)
{
	m_ShadowCamera = new CGameObject();
	m_ShadowCamera->AddComponent(new CTransform);
	m_ShadowCamera->AddComponent(new CCamera);
	m_ShadowCamera->GetCamera()->CheckLayerAll();
	UINT8 layerIndex = 4; // UI
	m_ShadowCamera->GetCamera()->CheckLayer(layerIndex);
	//layerIndex = 10; // Map
	//m_ShadowCamera->GetCamera()->CheckLayer(layerIndex);
	m_ShadowCamera->SetLayerIndex(0);
}

CLight::~CLight()
{
	if (m_ShadowCamera) {
		delete m_ShadowCamera;
		m_ShadowCamera = nullptr;
	}
}

void CLight::FinalUpdate()
{
	m_Light.position = GetTransform()->GetWorldPosition();

	UpdateCascadeShadowVP();

	m_ShadowCamera->GetTransform()->SetRelativePosition(GetTransform()->GetRelativePosition());
	m_ShadowCamera->GetTransform()->SetRelativeRotation(GetTransform()->GetRelativeRotation());
	m_ShadowCamera->GetTransform()->SetRelativeScale(GetTransform()->GetRelativeScale());
	m_ShadowCamera->GetTransform()->LookAt(Vec3(0.f, -1.f, 0.f));
	m_ShadowCamera->FinalUpdate();

}

void CLight::Render()
{
	assert(m_LightIndex >= 0);

	GetTransform()->GraphicsBinding();


	if (static_cast<LIGHT_TYPE>(m_Light.lightType) == LIGHT_TYPE::DIRECTIONAL_LIGHT)
	{
		m_CascadeShadowTex.clear();
		for (int i = 0; i < CASCADE_COUNT; ++i)
		{
			std::wstring name = L"ShadowTarget_" + std::to_wstring(i);
			m_CascadeShadowTex.push_back(CDevice::GetInst()->GetRenderTargetGroup(RENDER_TARGET_GROUP_TYPE::SHADOW)->GetRTTexture(i));
			m_CascadeShadowData.SetSplitDistances(i, m_SplitDepth[i + 1]);
			m_CascadeShadowData.SetMatCascadeVP(i, m_CascadeVP[i]);
		}
		PushCascadeData();
	}
	else
	{
		float scale = 2 * m_Light.range;
		GetTransform()->SetRelativeScale(Vec3(scale, scale, scale));
	}


	m_LightMaterial->SetInt(0, m_LightIndex);
	m_LightMaterial->GraphicsBinding();

	m_VolumeMesh->Render();	
}

void CLight::PushCascadeData()
{
	// CBV 업로드
	//CONST_BUFFER(EConstantBuffer_Type::CascadeShadowData)->SetGlobalData(&m_CascadeShadowData, sizeof(m_CascadeShadowData), 4);

	// SRV 업로드
	for (size_t i = 0; i < m_CascadeShadowTex.size(); i++)
	{
		if (m_CascadeShadowTex[i] == nullptr)
			continue;

		SRV_REGISTER reg = SRV_REGISTER(static_cast<char>(SRV_REGISTER::t8) + i);
		CDevice::GetInst()->GetGraphicsDescHeap()->SetSRV(m_CascadeShadowTex[i]->GetSRVCpuHandle(), reg);
	}

}

void CLight::RenderShadow()
{
	if (static_cast<LIGHT_TYPE>(m_Light.lightType) != LIGHT_TYPE::DIRECTIONAL_LIGHT)
		return;

	auto rtGroup = CDevice::GetInst()->GetRenderTargetGroup(RENDER_TARGET_GROUP_TYPE::SHADOW);
	if (!rtGroup)
		return;

	for (int i = 0; i < CASCADE_COUNT; ++i)
	{
		rtGroup->OMSetRenderTargets(1, i); 


		m_ShadowCamera->GetCamera()->SetView(m_CascadeView[i]);
		m_ShadowCamera->GetCamera()->SetProjection(m_CascadeProj[i]);
		m_ShadowCamera->GetCamera()->SortShadowObject(); 
		m_ShadowCamera->GetCamera()->RenderShadow();   
	}

}

void CLight::SetLightDirection(Vec3 direction)
{
	direction.Normalize();

	m_Light.direction = direction;

	GetTransform()->LookAt(direction);
}


void CLight::SetLightType(LIGHT_TYPE type)
{
	m_Light.lightType = static_cast<INT32>(type);

	switch (type)
	{
	case LIGHT_TYPE::DIRECTIONAL_LIGHT:
		m_VolumeMesh = CAssetManager::GetInst()->FindAsset<CMesh>(L"Rectangle");
		m_LightMaterial = CAssetManager::GetInst()->FindAsset<CMaterial>(L"DirLight");
		m_ShadowCamera->GetCamera()->SetScale(1.f);
		m_ShadowCamera->GetCamera()->SetFar(10000.f);
		m_ShadowCamera->GetCamera()->SetWidth(4096);
		m_ShadowCamera->GetCamera()->SetHeight(4096);
		break;
	case LIGHT_TYPE::POINT_LIGHT:
		m_VolumeMesh = CAssetManager::GetInst()->FindAsset<CMesh>(L"Sphere");
		m_LightMaterial = CAssetManager::GetInst()->FindAsset<CMaterial>(L"PointLight");
		break;
	case LIGHT_TYPE::SPOT_LIGHT:
		// 원뿔 메쉬 만들어야함
		m_VolumeMesh = CAssetManager::GetInst()->FindAsset<CMesh>(L"Sphere");
		m_LightMaterial = CAssetManager::GetInst()->FindAsset<CMaterial>(L"PointLight");
		break;
	}
}

void CLight::CalcCascadeSplits(float nearZ, float farZ)
{
	m_SplitDepth[0] = 1.f;      
	m_SplitDepth[1] = 500.f;     
	m_SplitDepth[2] = 1000.f;  
	m_SplitDepth[3] = 2000.f;   
	m_SplitDepth[4] = 5000.f;   
}

void CLight::UpdateCascadeShadowVP()
{
	if (static_cast<LIGHT_TYPE>(m_Light.lightType) != LIGHT_TYPE::DIRECTIONAL_LIGHT)
		return;

	Vec3 lightDir = -GetTransform()->GetWorldDir(EDir::Front);
	lightDir.Normalize();

	CCamera* mainCam = CRenderManager::GetInst()->GetMainCamera();
	const Matrix view = mainCam->GetViewMat();
	const Matrix proj = mainCam->GetProjMat();
	const Matrix viewToWorld = view.Invert();

	const float nearZ = mainCam->GetNear();
	const float farZ = mainCam->GetFar();
	const float fov = mainCam->GetFOV();
	const float aspect = mainCam->GetAspectRatio();

	CalcCascadeSplits(nearZ, farZ);
	CalcCascadeFrustumCornersAndVP(view, viewToWorld, fov, aspect, nearZ, farZ, lightDir);
}

void CLight::CalcCascadeFrustumCornersAndVP(const Matrix& view, const Matrix& viewToWorld, float fov, float aspect, float nearZ, float farZ, const Vec3& lightDir)
{
	// View Space Frustum 8 corners
	std::vector<Vec3> frustumCornersVS;
	frustumCornersVS.reserve(8);

	float tanFov = tanf(fov * 0.5f);
	float yLenNear = nearZ * tanFov;
	float xLenNear = yLenNear * aspect;
	float yLenFar = farZ * tanFov;
	float xLenFar = yLenFar * aspect;

	// Near plane
	for (int y = -1; y <= 1; y += 2)
	{
		for (int x = -1; x <= 1; x += 2)
		{
			frustumCornersVS.emplace_back(x * xLenNear, y * yLenNear, nearZ);
		}
	}

	// Far plane
	for (int y = -1; y <= 1; y += 2)
	{
		for (int x = -1; x <= 1; x += 2)
		{
			frustumCornersVS.emplace_back(x * xLenFar, y * yLenFar, farZ);
		}
	}

	// 각 Cascade에 대해 처리
	for (int i = 0; i < CASCADE_COUNT; ++i)
	{
		float splitNear = m_SplitDepth[i];
		float splitFar = m_SplitDepth[i + 1];

		std::vector<Vec3> cornersWS;
		cornersWS.reserve(8);

		for (int j = 0; j < 4; ++j)
		{
			float lerpNear = (splitNear - nearZ) / (farZ - nearZ);
			float lerpFar = (splitFar - nearZ) / (farZ - nearZ);

			Vec3 nearCorner = Vec3::Lerp(frustumCornersVS[j], frustumCornersVS[j + 4], lerpNear);
			Vec3 farCorner = Vec3::Lerp(frustumCornersVS[j], frustumCornersVS[j + 4], lerpFar);

			Vec4 ncWS = XMVector4Transform(Vec4(nearCorner.x, nearCorner.y, nearCorner.z, 1.0f), viewToWorld);
			Vec4 fcWS = XMVector4Transform(Vec4(farCorner.x, farCorner.y, farCorner.z, 1.0f), viewToWorld);

			cornersWS.emplace_back(ncWS.x, ncWS.y, ncWS.z);
			cornersWS.emplace_back(fcWS.x, fcWS.y, fcWS.z);
		}

		// AABB
		Vec3 minCorner = cornersWS[0];
		Vec3 maxCorner = cornersWS[0];
		for (size_t j = 1; j < cornersWS.size(); ++j)
		{
			minCorner = Vec3::Min(minCorner, cornersWS[j]);
			maxCorner = Vec3::Max(maxCorner, cornersWS[j]);
		}

		Vec3 center = (minCorner + maxCorner) * 0.5f;
		float radius = (maxCorner - minCorner).Length() * 0.5f;
		radius = max(radius, 50.f); // 안전 여유

		float minRadius = 1000.f * (i+1);
		radius = min(minRadius, radius);

		Vec3 eye = center - lightDir * radius;
		Vec3 up(0.f, 1.f, 0.f);

		Matrix lightView = XMMatrixLookAtLH(eye, center, up);
		Matrix lightProj = XMMatrixOrthographicLH(radius * 2.f, radius * 2.f, 0.0f, 5000.f);

		m_CascadeView[i] = lightView;
		m_CascadeProj[i] = lightProj;
		m_CascadeVP[i] = lightProj * lightView;
	}
}
