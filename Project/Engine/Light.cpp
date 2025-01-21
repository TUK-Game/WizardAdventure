#include "pch.h"
#include "Light.h"
#include "Transform.h"

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