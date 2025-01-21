#include "pch.h"
#include "Light.h"
#include "Transform.h"
#include "AssetManager.h"

CLight::CLight() : CComponent(EComponent_Type::Light)
{
}

CLight::~CLight()
{

}

void CLight::FinalUpdate()
{
	m_Light.position = GetTransform()->GetWorldPosition();
	//m_Light.direction = GetTransform()->GetWorldDir(EDir::Front);
}

void CLight::Render()
{
	assert(m_LightIndex >= 0);

	GetTransform()->GraphicsBinding();

	m_LightMaterial->SetInt(0, m_LightIndex);
	m_LightMaterial->GraphicsBinding();

	switch (static_cast<LIGHT_TYPE>(m_Light.lightType))
	{
	case LIGHT_TYPE::POINT_LIGHT:
	case LIGHT_TYPE::SPOT_LIGHT:
		float scale = 2 * m_Light.range;
		GetTransform()->SetRelativeScale(Vec3(scale, scale, scale));
		break;
	}

	m_VolumeMesh->Render();
}

void CLight::SetLightType(LIGHT_TYPE type)
{
	m_Light.lightType = static_cast<INT32>(type);

	switch (type)
	{
	case LIGHT_TYPE::DIRECTIONAL_LIGHT:
		m_VolumeMesh = CAssetManager::GetInst()->FindAsset<CMesh>(L"Rectangle");
		m_LightMaterial = CAssetManager::GetInst()->FindAsset<CMaterial>(L"DirLight");
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
