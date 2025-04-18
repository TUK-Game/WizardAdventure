#include "pch.h"
#include "ParticleSystem.h"
#include "StructuredBuffer.h"
#include "Mesh.h"
#include "AssetManager.h"
#include "Transform.h"
#include "Timer.h"
#include "Engine.h"

CParticleSystem::CParticleSystem() : CComponent(EComponent_Type::ParticleSystem)
{
	m_ParticleBuffer = std::make_shared<CStructuredBuffer>();
	m_ParticleBuffer->Init(sizeof(ParticleInfo), m_MaxParticle);

	m_ComputeSharedBuffer = std::make_shared<CStructuredBuffer>();
	m_ComputeSharedBuffer->Init(sizeof(ComputeSharedInfo), 1);

	m_Mesh = CAssetManager::GetInst()->FindAsset<CMesh>(L"Point");
	m_GraphicsMaterial = CAssetManager::GetInst()->FindAsset<CMaterial>(L"Particle");
	m_ComputeMaterial = CAssetManager::GetInst()->FindAsset<CMaterial>(L"ComputeParticle");

	CTexture* tex = CAssetManager::GetInst()->FindAsset<CTexture>(L"Spark");
	m_GraphicsMaterial->SetTexture(0, tex);

}

CParticleSystem::CParticleSystem(UINT32 maxParticle) : CComponent(EComponent_Type::ParticleSystem)
{
	m_MaxParticle = maxParticle;
	m_ParticleBuffer = std::make_shared<CStructuredBuffer>();
	m_ParticleBuffer->Init(sizeof(ParticleInfo), m_MaxParticle);

	m_ComputeSharedBuffer = std::make_shared<CStructuredBuffer>();
	m_ComputeSharedBuffer->Init(sizeof(ComputeSharedInfo), 1);

	m_Mesh = CAssetManager::GetInst()->FindAsset<CMesh>(L"Point");
	m_GraphicsMaterial = CAssetManager::GetInst()->FindAsset<CMaterial>(L"Particle");
	m_ComputeMaterial = CAssetManager::GetInst()->FindAsset<CMaterial>(L"ComputeParticle");
}

CParticleSystem::~CParticleSystem()
{
	//m_GraphicsMaterial->ReleaseRef();
	//m_ComputeMaterial->ReleaseRef();
}

void CParticleSystem::SetTexture(CTexture* texture)
{
	m_GraphicsMaterial->SetTexture(0, texture);
}

void CParticleSystem::SetTexture(const std::wstring& name)
{
	m_GraphicsMaterial->SetTexture(0, CAssetManager::GetInst()->FindAsset<CTexture>(name));
}

void CParticleSystem::FinalUpdate()
{
	m_AccTime += DELTA_TIME;

	INT32 add = 0;
	if (m_bEmit)
	{
		if (_createInterval < m_AccTime)
		{
			m_AccTime = m_AccTime - _createInterval;
			add = 1;
		}
	}

	m_ParticleBuffer->PushComputeUAVData(UAV_REGISTER::u0);
	m_ComputeSharedBuffer->PushComputeUAVData(UAV_REGISTER::u1);

	m_ComputeMaterial->SetInt(0, m_MaxParticle);
	m_ComputeMaterial->SetInt(1, add);

	m_ComputeMaterial->SetVec2(1, Vec2(DELTA_TIME, m_AccTime));
	m_ComputeMaterial->SetVec4(0, Vec4(_minLifeTime, _maxLifeTime, _minSpeed, _maxSpeed));

	m_ComputeMaterial->SetVec4(1, Vec4(m_BasePos.x, m_BasePos.y, m_BasePos.z, 0));
	m_ComputeMaterial->Dispatch(1, 1, 1);
}

void CParticleSystem::Render()
{
	GetTransform()->GraphicsBinding();

	m_ParticleBuffer->PushGraphicsData(SRV_REGISTER::t9);
	m_GraphicsMaterial->SetFloat(0, _startScale);
	m_GraphicsMaterial->SetFloat(1, _endScale);
	m_GraphicsMaterial->GraphicsBinding();

	m_Mesh->Render(m_MaxParticle);
}
