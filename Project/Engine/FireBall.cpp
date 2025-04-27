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
	m_FireParticle = CParticleSystemManager::GetInst()->Request();
	if (m_FireParticle)
		m_FireParticle->GetParticleSystem()->SetTexture(L"Spark");
}

void CFireBall::Update()
{
	UpdateByMode();
	CSkillObject::Update();
}

void CFireBall::FinalUpdate()
{
	Vec3 pos = GetTransform()->GetRelativePosition();
	if (m_FireParticle)
		m_FireParticle->GetParticleSystem()->SetBasePos(pos);
	if (m_SmokeParticle)
		m_SmokeParticle->GetParticleSystem()->SetBasePos(pos);

	CGameObject::FinalUpdate();

	if (m_bOwn)
	{
		m_ElapsedTime += DELTA_TIME;
		if (m_ElapsedTime >= m_Duration && !m_bDelete) {
			SpawnDeleteEffect();
			m_bDelete = true;
		}

		if (pos.y < -20.f && !m_bDelete) {
			if (m_FireParticle) {
				CParticleSystemManager::GetInst()->Return(m_FireParticle);
				m_FireParticle = nullptr;
			}
			if (m_SmokeParticle) {
				CParticleSystemManager::GetInst()->Return(m_SmokeParticle);
				m_SmokeParticle = nullptr;
			}
			CEffectManager::GetInst()->SpawnRadialSmoke(pos);
			CEffectManager::GetInst()->SpawnEffect(L"Explosion", pos);
			CEffectManager::GetInst()->SpawnEffect(L"Explosion1", pos);
			CEffectManager::GetInst()->SpawnEffect(L"Shockwave", pos);
			m_bDelete = true;
		}
	}

}

void CFireBall::CollisionBegin(CBaseCollider* src, CBaseCollider* dest)
{
	CSkillObject::CollisionBegin(src, dest);
}

void CFireBall::UseSmokeTrail()
{
	m_SmokeParticle = CParticleSystemManager::GetInst()->Request();
	if (m_SmokeParticle)
		m_SmokeParticle->GetParticleSystem()->SetTexture(L"Smoke");

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
	Vec3 pos = GetTransform()->GetRelativePosition();
	switch (m_Mode) {
	case EFireBallMode::Default:
		break;
	case EFireBallMode::QSkill:
		CEffectManager::GetInst()->SpawnEffect(L"Explosion", pos);
		CEffectManager::GetInst()->SpawnEffect(L"Explosion1", pos);
		CEffectManager::GetInst()->SpawnEffect(L"Shockwave", pos);
		break;
	case EFireBallMode::Meteor:
		CEffectManager::GetInst()->SpawnRadialSmoke(pos);
		CEffectManager::GetInst()->SpawnEffect(L"Explosion", pos);
		CEffectManager::GetInst()->SpawnEffect(L"Explosion1", pos);
		CEffectManager::GetInst()->SpawnEffect(L"Shockwave", pos);
		std::cout << "터지는 효과 발동\n";
		break;
	}
}


void CFireBall::UpdateScaleLerp()
{
	float t = m_ElapsedTime / m_Duration;
	t = std::clamp(t, 0.f, 1.f);

	Vec3 newScale = Vec3::Lerp(m_StartScale, m_EndScale, t);
	GetTransform()->SetRelativeScale(newScale);
}