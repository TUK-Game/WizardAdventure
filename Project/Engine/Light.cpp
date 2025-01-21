#include "pch.h"
#include "Light.h"
#include "Transform.h"

CLight::CLight() : CComponent(EComponent_Type::Light)
{
    // 기본값 초기화
    m_Light = {};
    m_Light.m_bEnable = true;
    m_Light.m_nType = 0; // Default: Directional Light
    m_Light.m_xmf4Ambient = XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
    m_Light.m_xmf4Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
    m_Light.m_xmf4Specular = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
    m_Light.m_xmf3Direction = XMFLOAT3(0.0f, -1.0f, 0.0f);
    m_Light.m_fRange = 100.0f;
    m_Light.m_fFalloff = 1.0f;
    m_Light.m_fTheta = cosf(XM_PIDIV4); // 45도
    m_Light.m_fPhi = cosf(XM_PIDIV2);  // 90도
    m_Light.m_xmf3Attenuation = XMFLOAT3(1.0f, 0.0f, 0.0f);
}

CLight::~CLight()
{

}

void CLight::FinalUpdate()
{
	m_Light.m_xmf3Position = GetTransform()->GetWorldPosition();
	m_Light.m_xmf3Direction = GetTransform()->GetWorldDir(EDir::Front);
}