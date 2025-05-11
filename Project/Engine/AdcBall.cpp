#include "pch.h"
#include "AdcBall.h"
#include "Transform.h"
#include "MeshRenderer.h"
#include "RigidBody.h"
#include "AssetManager.h"
#include "Level.h"
#include "Layer.h"
#include "LevelManager.h"
#include "Engine.h"
#include "ParticleSystem.h"
#include "ParticleSystemManager.h"

CAdcBall::CAdcBall()
{

	AddComponent(new CTransform());
	AddComponent(new CMeshRenderer());
	GetTransform()->SetRelativeScale(20.f, 20.f, 20.f);
	GetMeshRenderer()->SetMesh(CAssetManager::GetInst()->FindAsset<CMesh>(L"Sphere"));
	GetMeshRenderer()->SetMaterial(CAssetManager::GetInst()->FindAsset<CMaterial>(L"LightBack"));
	AddComponent(new CRigidBody());
	GetRigidBody()->SetGravity(true);

	// AddComponent(new CCollider());      
	m_LightParticleId = CParticleSystemManager::GetInst()->AddEmitter(L"Light", GetTransform()->GetRelativePosition());

}

void CAdcBall::Update()
{
	CSkillObject::Update();
}

void CAdcBall::FinalUpdate()
{
	Vec3 pos = GetTransform()->GetRelativePosition();

	if (0 <= m_LightParticleId)
		CParticleSystemManager::GetInst()->UpdateEmitterPos(L"Light", m_LightParticleId, pos);

	CSkillObject::FinalUpdate();

	if (m_bOwn)
	{
		m_ElapsedTime += DELTA_TIME;
		std::cout << m_ElapsedTime << '\n';
		if (m_ElapsedTime >= m_Duration)
		{
			m_bDelete = true;
			std::cout << "삭제진행\n";
		}
	}
}

void CAdcBall::CollisionBegin(CBaseCollider* src, CBaseCollider* dest)
{
	//CSkillObject::CollisionBegin(src, dest);
}

void CAdcBall::OffParticles()
{
	if (0 <= m_LightParticleId)
	{
		CParticleSystemManager::GetInst()->RemoveEmitter(L"Light", m_LightParticleId);
		m_LightParticleId = -1;
	}
	std::cout << "삭제\n";
}
