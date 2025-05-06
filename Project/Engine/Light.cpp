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
	m_LightMaterial->SetFloat(0, m_SplitDepth[1]);
	m_LightMaterial->SetFloat(1, m_SplitDepth[2]);
	m_LightMaterial->SetFloat(2, m_SplitDepth[3]);
	m_LightMaterial->SetFloat(3, m_SplitDepth[4]);
	m_LightMaterial->SetMatrix(0, m_CascadeVP[0]);
	m_LightMaterial->SetMatrix(1, m_CascadeVP[1]);
	m_LightMaterial->SetMatrix(2, m_CascadeVP[2]);
	m_LightMaterial->SetMatrix(3, m_CascadeVP[3]);
	m_LightMaterial->GraphicsBinding();

	m_VolumeMesh->Render();	
}

void CLight::PushCascadeData()
{
	// CBV 업로드
	CONST_BUFFER(EConstantBuffer_Type::CascadeShadowData)->SetGlobalData(&m_CascadeShadowData, sizeof(m_CascadeShadowData), 2);

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
		m_ShadowCamera->GetCamera()->SetFar(5000.f);
		m_ShadowCamera->GetCamera()->SetWidth(2048);
		m_ShadowCamera->GetCamera()->SetHeight(2048);
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

void CLight::UpdateCascadeShadowVP()
{
	if (static_cast<LIGHT_TYPE>(m_Light.lightType) != LIGHT_TYPE::DIRECTIONAL_LIGHT)
		return;

	Vec3 lightDir = -GetTransform()->GetWorldDir(EDir::Front);
	lightDir.Normalize();

	CCamera* mainCam = CRenderManager::GetInst()->GetMainCamera();
	const Matrix view = mainCam->GetViewMat();
	const Matrix proj = mainCam->GetProjMat();
	const float farZ = mainCam->GetFar();
	const Matrix viewProj = view * proj;
	const Matrix invViewProj = viewProj.Invert();

	// NDC 공간에서의 프러스텀 정점
	Vec3 FrustumCorners[8] = {
		Vec3(-1.0f,  1.0f, 0.0f), // Near Top Left
		Vec3(1.0f,  1.0f, 0.0f), // Near Top Right
		Vec3(1.0f, -1.0f, 0.0f), // Near Bottom Right
		Vec3(-1.0f, -1.0f, 0.0f), // Near Bottom Left
		Vec3(-1.0f,  1.0f, 1.0f), // Far Top Left
		Vec3(1.0f,  1.0f, 1.0f), // Far Top Right
		Vec3(1.0f, -1.0f, 1.0f), // Far Bottom Right
		Vec3(-1.0f, -1.0f, 1.0f)  // Far Bottom Left
	};
	
	// NDC -> World
	for (auto& v : FrustumCorners)
		v = Vec3::Transform(v, invViewProj);


	constexpr float cascadeSplits[5] = { 0.0f, 0.1f, 0.2f, 0.4f, 1.0f };
	for (int i = 0; i < 5; ++i) 
	{
		m_SplitDepth[i] = farZ * cascadeSplits[i];

	}
	for (int i = 0; i < CASCADE_COUNT; ++i)
	{

		Vec3 tFrustum[8];
		for (int j = 0; j < 8; ++j)
			tFrustum[j] = FrustumCorners[j];

		// 프러스텀 계산
		for (int j = 0; j < 4; ++j)
		{
			// 앞 -> 뒤 벡터
			Vec3 v = (tFrustum[j + 4] - tFrustum[j]);
			
			Vec3 n = v * cascadeSplits[i];
			Vec3 f = v * cascadeSplits[i+1];

			tFrustum[j + 4] = tFrustum[j] + f;
			tFrustum[j] = tFrustum[j] + n;

		}

		// 바운딩 구 중심 계산
		Vec3 center{};
		for (const auto& v : tFrustum)
			center += v;
		center = center / 8.f;

		// 바운딩 구 반지름 계산
		float radius{};
		for (const auto& v : tFrustum)
			radius = max(radius, (v - center).Length());


		float value{ min(5000.0f, radius * 2.5f) };
		Vec3 shadowLightPos = center + (lightDir * value);

		Matrix lightViewMatrix, lightProjMatrix;
		Vec3 up{ 0.0f, 1.0f, 0.0f };
		lightViewMatrix = XMMatrixLookAtLH(shadowLightPos, center, up);
		lightProjMatrix = XMMatrixOrthographicLH(radius * 2.0f, radius * 2.0f, 0.0f, 5000.0f);

		m_CascadeView[i] = lightViewMatrix;
		m_CascadeProj[i] = lightProjMatrix;
		m_CascadeVP[i] = lightProjMatrix * lightViewMatrix;
	}
}
