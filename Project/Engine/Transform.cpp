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
	// ������ķ� ��ȯ
	// Scale 
	Matrix matScale = XMMatrixScaling(m_RelativeScale.x, m_RelativeScale.y, m_RelativeScale.z);

	// Rotation
	Matrix matRotation = XMMatrixRotationX(m_RelativeRotation.x);
	matRotation *= XMMatrixRotationY(m_RelativeRotation.y);
	matRotation *= XMMatrixRotationZ(m_RelativeRotation.z);

	// Translation
	Matrix matTrans = XMMatrixTranslation(m_RelativePos.x, m_RelativePos.y, m_RelativePos.z);

	// ũ�� ȸ�� �̵� �θ� ������ ����
	m_matWorld = matScale * matRotation * matTrans;

	// ������Ʈ�� �������� ���
	m_RelativeDir[(UINT)EDir::Right] = Vec3(1.f, 0.f, 0.f);
	m_RelativeDir[(UINT)EDir::Up] = Vec3(0.f, 1.f, 0.f);
	m_RelativeDir[(UINT)EDir::Front] = Vec3(0.f, 0.f, 1.f);

	// ���⺤�Ϳ� ȸ������� �����ؼ� ���� ���Ⱚ�� ���
	for (int i = 0; i < (int)EDir::END; ++i)
	{
		m_WorldDir[i] = m_RelativeDir[i] = XMVector3TransformNormal(m_RelativeDir[i], matRotation);
	}

	// TODO: �θ� ������Ʈ�� �ִٸ�, �θ��� ������� ����
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