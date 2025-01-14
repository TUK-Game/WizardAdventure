#include "pch.h"
#include "Transform.h"
#include "Device.h"
#include "Camera.h"

CTransform::CTransform()
	: CComponent(EComponent_Type::Transform)
{
}

CTransform::~CTransform()
{
}

void CTransform::FinalUpdate()
{
	// 월드행렬로 변환
	// Scale 
	Matrix matScale = XMMatrixScaling(m_RelativeScale.x, m_RelativeScale.y, m_RelativeScale.z);

	// Rotation
	Matrix matRotation = XMMatrixRotationX(m_RelativeRotation.x);
	matRotation *= XMMatrixRotationY(m_RelativeRotation.y);
	matRotation *= XMMatrixRotationZ(m_RelativeRotation.z);

	// Translation
	Matrix matTrans = XMMatrixTranslation(m_RelativePos.x, m_RelativePos.y, m_RelativePos.z);

	// 크기 회전 이동 부모 순서로 적용
	m_matWorld = matScale * matRotation * matTrans;
	m_matRT = matRotation * matTrans;

	// 오브젝트의 방향정보 계산
	m_RelativeDir[(UINT)EDir::Right] = Vec3(1.f, 0.f, 0.f);
	m_RelativeDir[(UINT)EDir::Up] = Vec3(0.f, 1.f, 0.f);
	m_RelativeDir[(UINT)EDir::Front] = Vec3(0.f, 0.f, 1.f);

	// 방향벡터에 회전행렬을 적용해서 현재 방향값을 계산
	for (int i = 0; i < (int)EDir::END; ++i)
	{
		m_WorldDir[i] = m_RelativeDir[i] = XMVector3TransformNormal(m_RelativeDir[i], matRotation);
	}

	// TODO: 부모 오브젝트가 있다면, 부모의 월드행렬 누적
	if (m_ParentTransform) // 부모 오브젝트가 존재하면
	{
		m_matWorld = m_matWorld * m_ParentTransform->GetRTMatrix();
		m_matRT = m_matRT * m_ParentTransform->GetRTMatrix();

		// 부모의 월드 방향 벡터를 기준으로 자식 오브젝트의 월드 방향 벡터 계산
		for (int i = 0; i < (int)EDir::END; ++i)
		{
			m_WorldDir[i] = XMVector3TransformNormal(m_RelativeDir[i], m_ParentTransform->GetRTMatrix());
		}
	}
}

void CTransform::Binding()
{
	Matrix matWVP = m_matWorld * CCamera::s_matView * CCamera::s_matProjection;
	CONST_BUFFER(EConstantBuffer_Type::Transform)->PushData(&matWVP, sizeof(matWVP));
}

Vec3 CTransform::GetRelativeRotation()
{
	Vec3 rotation = (m_RelativeRotation / XM_PI) * 180.f;
	return rotation;
}

Vec3 CTransform::GetWorldPosition()
{
	return Vec3();
}

Vec3 CTransform::GetWorldScale()
{
	return Vec3();
}

Vec3 CTransform::GetWorldRotation()
{
	return Vec3();
}

void CTransform::SetRelativeRotation(Vec3 rotation)
{
	m_RelativeRotation = (rotation / 180.f) * XM_PI;
}

void CTransform::SetRelativeRotation(float x, float y, float z)
{
	m_RelativeRotation = (Vec3(x, y, z) / 180.f) * XM_PI;
}