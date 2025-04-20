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
    m_ParticleObject = CParticleSystemManager::GetInst()->Request();
}

void CFireBall::Update()
{
    CSkillObject::Update();
}

void CFireBall::FinalUpdate()
{
    if (m_ParticleObject)
        m_ParticleObject->GetParticleSystem()->SetBasePos(GetTransform()->GetRelativePosition());

    CGameObject::FinalUpdate();

    if (m_bOwn)
    {
    
        m_ElapsedTime += DELTA_TIME;
        if (m_ElapsedTime >= m_Duration) {
            m_bDelete = true;
        }
    }

    auto pos = GetTransform()->GetRelativePosition();

    if (pos.y < 10.f) {
        if (m_ParticleObject) {
            CParticleSystemManager::GetInst()->Return(m_ParticleObject);
            m_ParticleObject = nullptr;
        }
        CParticleSystemManager::GetInst()->RequestExplodeAt(pos);
        CEffectManager::GetInst()->SpawnEffect(L"Explosion", pos);
        CEffectManager::GetInst()->SpawnEffect(L"Shockwave", pos);
        CLevelManager::GetInst()->GetCurrentLevel()->GetLayer(GetLayerIndex())->SafeRemoveGameObject(this);
    }


}

void CFireBall::CollisionBegin(CBaseCollider* src, CBaseCollider* dest)
{
    CSkillObject::CollisionBegin(src, dest); 
}