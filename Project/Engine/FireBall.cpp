#include "pch.h"
#include "FireBall.h"
#include "Transform.h"
#include "MeshRenderer.h"
#include "RigidBody.h"
#include "AssetManager.h"
#include "Level.h"
#include "Layer.h"
#include "LevelManager.h"
#include "ParticleSystem.h"
#include "ParticleSystemManager.h"
#include "EffectManager.h"
#include "Engine.h"
#include "NetworkManager.h"
#include "ServerSession.h"

CFireBall::CFireBall()
	: m_Speed(1000.f)
{
	m_type = SKILL::FIRE_BALL;

	AddComponent(new CTransform());
	AddComponent(new CMeshRenderer());
	GetTransform()->SetRelativeScale(30.f, 30.f, 30.f);
	GetMeshRenderer()->SetMesh(CAssetManager::GetInst()->FindAsset<CMesh>(L"Sphere"));
	GetMeshRenderer()->SetMaterial(CAssetManager::GetInst()->FindAsset<CMaterial>(L"Lava"));
	AddComponent(new CRigidBody());

	// AddComponent(new CCollider());      
	m_FireParticleId = CParticleSystemManager::GetInst()->AddEmitter(L"Spark", GetTransform()->GetRelativePosition());
}

void CFireBall::Update()
{
	if(m_bOwn)
	{
		UpdateByMode();
	}
	CSkillObject::Update();
}

void CFireBall::FinalUpdate()
{
	Vec3 pos = GetTransform()->GetRelativePosition();
	if (0 <= m_FireParticleId)
		CParticleSystemManager::GetInst()->UpdateEmitterPos(L"Spark", m_FireParticleId, pos);
	if (0 <= m_SmokeParticleId)
		CParticleSystemManager::GetInst()->UpdateEmitterPos(L"Smoke", m_SmokeParticleId, pos);

	CGameObject::FinalUpdate();

	if (m_bOwn)
	{
		m_ElapsedTime += DELTA_TIME;
		if (m_ElapsedTime >= m_Duration)
		{
			m_bDelete = true;
		}


		if (pos.y < -1000.f && !m_bDelete)
		{
			m_bDelete = true;
		}
	}

}

void CFireBall::CollisionBegin(CBaseCollider* src, CBaseCollider* dest)
{
	CSkillObject::CollisionBegin(src, dest);
}

void CFireBall::ShowParticles()
{

}

void CFireBall::OffParticles()
{
	Vec3 pos = GetTransform()->GetRelativePosition();

	if (0 <= m_FireParticleId)
	{
		CParticleSystemManager::GetInst()->RemoveEmitter(L"Spark", m_FireParticleId);
		m_FireParticleId = -1;
	}

	switch (m_Mode) {
	case EFireBallMode::Default:
	{
	}
	break;
	case EFireBallMode::QSkill:
	{
		CAssetManager::GetInst()->SoundPlay("FireBallQExplosion");
		CEffectManager::GetInst()->SpawnEffect(L"Explosion", pos);
		CEffectManager::GetInst()->SpawnEffect(L"Explosion1", pos);
		CEffectManager::GetInst()->SpawnEffect(L"Shockwave", pos);
		std::cout << "ss\n";
	}
	break;
	case EFireBallMode::Meteor:
	{
		if (0 <= m_SmokeParticleId)
		{
			CParticleSystemManager::GetInst()->RemoveEmitter(L"Smoke", m_SmokeParticleId);
			m_SmokeParticleId = -1;
		}
		CAssetManager::GetInst()->SoundPlay("MeteorExplosion");
		CEffectManager::GetInst()->SpawnRadialSmoke(pos);
		CEffectManager::GetInst()->SpawnEffect(L"Explosion", pos);
		CEffectManager::GetInst()->SpawnEffect(L"Explosion1", pos);
		CEffectManager::GetInst()->SpawnEffect(L"Shockwave", pos);
	}
	break;
	}
}

void CFireBall::UseSmokeTrail()
{
	m_SmokeParticleId = CParticleSystemManager::GetInst()->AddEmitter(L"Smoke", GetTransform()->GetRelativePosition());
}

void CFireBall::UpdateByMode()
{
	switch (m_Mode) {
	case EFireBallMode::Default:
		break;
	case EFireBallMode::QSkill:
		UpdateScaleLerp();
		break;
	case EFireBallMode::Meteor:
		break;
	}
}

void CFireBall::SpawnDeleteEffect()
{

}


void CFireBall::UpdateScaleLerp()
{
	float t = m_ElapsedTime / m_Duration;
	t = std::clamp(t, 0.f, 1.f);

	Vec3 newScale = Vec3::Lerp(m_StartScale, m_EndScale, t);
	GetTransform()->SetRelativeScale(newScale);
}