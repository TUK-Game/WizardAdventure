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
	InitBuffers(m_MaxParticle, m_MaxEmitter);

	m_Mesh = CAssetManager::GetInst()->FindAsset<CMesh>(L"Point");
	m_GraphicsMaterial = CAssetManager::GetInst()->FindAsset<CMaterial>(L"Particle")->Clone();
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


void CParticleSystem::InitBuffers(UINT32 maxParticle, UINT32 maxEmitter)
{
	m_ParticleBuffer = std::make_shared<CStructuredBuffer>();
	m_ParticleBuffer->Init(sizeof(ParticleInfo), maxParticle);

	m_EmitterBuffer = std::make_shared<CStructuredBuffer>();
	m_EmitterBuffer->Init(sizeof(EmitterInfo), maxEmitter);

	m_ComputeSharedBuffer = std::make_shared<CStructuredBuffer>();
	m_ComputeSharedBuffer->Init(sizeof(ComputeSharedInfo), 1);
}

int CParticleSystem::AddEmitter(const Vec3& basePos)
{
	for (int i = 0; i < m_Emitters.size(); ++i)
	{
		if (m_Emitters[i].isAlive < 0.5f) // 비활성화 된 emitter 재사용
		{
			m_Emitters[i].basePos = basePos;
			m_Emitters[i].isAlive = 1.f;
			return i;
		}
	}

	// 없으면 새로 추가
	m_Emitters.push_back({ basePos, 1.f });
	return static_cast<int>(m_Emitters.size() - 1);
}

void CParticleSystem::UpdateEmitterPos(int emitterID, const Vec3& newPos)
{
	if (emitterID < 0 || emitterID >= (int)m_Emitters.size())
		return;

	m_Emitters[emitterID].basePos = newPos;
}

void CParticleSystem::RemoveEmitter(int emitterID)
{
	if (emitterID < 0 || emitterID >= (int)m_Emitters.size())
		return;

	m_Emitters[emitterID].isAlive = 0.f; // 비활성화만 한다
}

void CParticleSystem::ClearEmitters()
{
	m_Emitters.clear();
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

	std::vector<EmitterInfo> aliveEmitters;

	for (const auto& emitter : m_Emitters)
	{
		if (emitter.isAlive > 0.5f)
			aliveEmitters.push_back(emitter);
	}


	if (m_bEmit)
	{
		if (m_CreateInterval < m_AccTime)
		{
			m_AccTime -= m_CreateInterval;
			add = aliveEmitters.size();
		}
	}

	m_EmitterBuffer->UpdateData(aliveEmitters.data(), static_cast<UINT32>(aliveEmitters.size()));
	m_ComputeSharedBuffer->UpdateData(nullptr, 1);

	m_EmitterBuffer->PushComputeSRVData(SRV_REGISTER::t6);
	m_ParticleBuffer->PushComputeUAVData(UAV_REGISTER::u0);
	m_ComputeSharedBuffer->PushComputeUAVData(UAV_REGISTER::u1);

	m_ComputeMaterial->SetInt(0, m_MaxParticle);
	m_ComputeMaterial->SetInt(1, add);
	m_ComputeMaterial->SetInt(3, (int)aliveEmitters.size());

	m_ComputeMaterial->SetVec2(1, Vec2(DELTA_TIME, m_AccTime));
	m_ComputeMaterial->SetVec4(0, Vec4(m_MinLifeTime, m_MaxLifeTime, m_MinSpeed, m_MaxSpeed));

	m_ComputeMaterial->Dispatch((m_MaxParticle + 1023) / 1024, 1, 1);
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

