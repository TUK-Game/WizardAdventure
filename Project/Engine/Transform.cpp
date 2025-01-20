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
	TransformParams transformParams = {};
	transformParams.matWorld = m_matWorld;
	transformParams.matView = CCamera::s_matView;
	transformParams.matProjection = CCamera::s_matProjection;
	transformParams.matWV = m_matWorld * CCamera::s_matView;
	transformParams.matWVP = m_matWorld * CCamera::s_matView * CCamera::s_matProjection;
	CONST_BUFFER(EConstantBuffer_Type::Transform)->PushData(&transformParams, sizeof(transformParams));
}

Vec3 CTransform::GetRelativeRotation()
{
	Vec3 rotation = (m_RelativeRotation / XM_PI) * 180.f;
	return rotation;
}

Vec3 CTransform::GetWorldPosition()
{
	return Vec3(m_matWorld._41, m_matWorld._42, m_matWorld._43);
}

Vec3 CTransform::GetWorldScale()
{
	return Vec3();
}

Vec3 CTransform::GetWorldRotation()
{
	Matrix worldMatrix = GetWorldMatrix();

	// 월드 행렬을 분해하여 스케일, 회전 Quaternion, 위치 추출
	XMVECTOR scale, rotationQuat, translation;
	XMMatrixDecompose(&scale, &rotationQuat, &translation, XMLoadFloat4x4(&worldMatrix));

	// 축-각도 추출
	XMVECTOR axis;
	float angle;
	XMQuaternionToAxisAngle(&axis, &angle, rotationQuat);

	// 축과 각도를 사용해 회전 벡터 생성 (각도를 곱함)
	XMFLOAT3 axisFloat3;
	XMStoreFloat3(&axisFloat3, axis);
	Vec3 rotation(axisFloat3.x * angle, axisFloat3.y * angle, axisFloat3.z * angle);

	return rotation;
}

Matrix CTransform::GetWorldRotationMatrix()
{
	// 월드 변환 행렬
	Matrix worldMatrix = GetWorldMatrix();

	// 월드 행렬을 분해하여 스케일, 회전 Quaternion, 위치 추출
	XMVECTOR scale, rotationQuat, translation;
	XMMatrixDecompose(&scale, &rotationQuat, &translation, XMLoadFloat4x4(&worldMatrix));

	// 회전 Quaternion을 회전 행렬로 변환
	Matrix rotationMatrix = XMMatrixRotationQuaternion(rotationQuat);

	return rotationMatrix;
}


void CTransform::SetRelativeRotation(Vec3 rotation)
{
	m_RelativeRotation = (rotation / 180.f) * XM_PI;
}

void CTransform::SetRelativeRotation(float x, float y, float z)
{
	m_RelativeRotation = (Vec3(x, y, z) / 180.f) * XM_PI;
}