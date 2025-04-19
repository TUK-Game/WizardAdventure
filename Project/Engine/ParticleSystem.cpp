#include "pch.h"
#include "ParticleSystem.h"
#include "ParticleSystemManager.h"
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
	if (m_bExplosionMode)
	{
		m_ExplosionElapsed += DELTA_TIME;

		if (m_bEmit)
		{
			add = 30; 
			m_bEmit = false; 
		}

		if (m_ExplosionElapsed >= m_ExplosionDuration)
		{
			m_bExplosionMode = false;
			m_ExplosionElapsed = 0.f;
			CParticleSystemManager::GetInst()->Return(GetOwner());
		}
	}

	if (m_bEmit)
	{
		if (m_CreateInterval < m_AccTime)
		{
			m_AccTime -= m_CreateInterval;
			add = 1;
		}
	}

	m_ParticleBuffer->PushComputeUAVData(UAV_REGISTER::u0);
	m_ComputeSharedBuffer->PushComputeUAVData(UAV_REGISTER::u1);

	m_ComputeMaterial->SetInt(0, m_MaxParticle);
	m_ComputeMaterial->SetInt(1, add);

	m_ComputeMaterial->SetVec2(1, Vec2(DELTA_TIME, m_AccTime));
	m_ComputeMaterial->SetVec4(0, Vec4(m_MinLifeTime, m_MaxLifeTime, m_MinSpeed, m_MaxSpeed));

	m_ComputeMaterial->SetVec4(1, Vec4(m_BasePos.x, m_BasePos.y, m_BasePos.z, 0));
	m_ComputeMaterial->Dispatch(1, 1, 1);
}

void CParticleSystem::Render()
{
	GetTransform()->GraphicsBinding();

	m_ParticleBuffer->PushGraphicsData(SRV_REGISTER::t9);
	m_GraphicsMaterial->SetFloat(0, m_StartScale);
	m_GraphicsMaterial->SetFloat(1, m_EndScale);
	m_GraphicsMaterial->GraphicsBinding();

	m_Mesh->Render(m_MaxParticle);
}

void CParticleSystem::ExplodeAt(const Vec3& pos)
{
	m_bExplosionMode = true;
	m_bEmit = true;
	m_BasePos = pos;
	m_AccTime = 0.f;
	m_ExplosionElapsed = 0.f;
	m_CreateInterval = 0.f; 
}

