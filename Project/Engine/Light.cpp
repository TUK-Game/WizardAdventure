#include "pch.h"
#include "Light.h"
#include "Transform.h"
#include "Camera.h"
#include "AssetManager.h"
#include "RenderManager.h"

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
		CTexture* shadowTex = CAssetManager::GetInst()->FindAsset<CTexture>(L"ShadowTarget");
		m_LightMaterial->SetTexture(2, shadowTex);

		Matrix matVP = m_ShadowCamera->GetCamera()->GetViewMat() * m_ShadowCamera->GetCamera()->GetProjMat();
		m_LightMaterial->SetMatrix(0, matVP);
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

void CLight::RenderShadow()
{
	m_ShadowCamera->GetCamera()->SortShadowObject();
	UpdateShadowVP();
	m_ShadowCamera->GetCamera()->RenderShadow();
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

void CLight::UpdateShadowVP()
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

	Vec3 center{};

	CGameObject* target = mainCam->GetTarget();
	if (target)
	{
		Vec3 targetPos = target->GetTransform()->GetWorldPosition();
		Vec3 viewPos = Vec3::Transform(targetPos, view);
		center = targetPos;
	}

	float radius = 2500.f;

	Vec3 shadowLightPos = center + (lightDir * radius);

	Matrix lightViewMatrix, lightProjMatrix;
	Vec3 up{ 0.0f, 1.0f, 0.0f };
	lightViewMatrix = XMMatrixLookAtLH(shadowLightPos, center, up);
	lightProjMatrix = XMMatrixOrthographicLH(radius * 2.0f, radius * 2.0f, 0.0f, 5000.0f);

	m_ShadowCamera->GetCamera()->SetView(lightViewMatrix);
	m_ShadowCamera->GetCamera()->SetProjection(lightProjMatrix);
}