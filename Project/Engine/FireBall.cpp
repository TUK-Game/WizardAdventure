#include "pch.h"
#include "FireBall.h"
#include "Transform.h"
#include "MeshRenderer.h"
#include "RigidBody.h"
#include "AssetManager.h"
#include "Level.h"
#include "Layer.h"
#include "LevelManager.h"
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
}

void CFireBall::Update()
{
    CSkillObject::Update();
}

void CFireBall::FinalUpdate()
{
    CGameObject::FinalUpdate();
    if (m_bOwn)
    {
        m_ElapsedTime += DELTA_TIME;
        if (m_ElapsedTime >= m_Duration) {
            //CLevelManager::GetInst()->GetCurrentLevel()->GetLayer(GetLayerIndex())->SafeRemoveGameObject(this);
            m_bDelete = true;
        }
    }
}

void CFireBall::CollisionBegin(CBaseCollider* src, CBaseCollider* dest)
{
    CSkillObject::CollisionBegin(src, dest); 
}