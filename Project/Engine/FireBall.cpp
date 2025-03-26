#include "pch.h"
#include "FireBall.h"
#include "Transform.h"
#include "MeshRenderer.h"
#include "RigidBody.h"
#include "AssetManager.h"
#include "Level.h"
#include "LevelManager.h"
#include "Engine.h"

CFireBall::CFireBall()
    : m_Speed(1000.f)
{
    AddComponent(new CTransform());
    AddComponent(new CMeshRenderer());  
    GetTransform()->SetRelativeScale(30.f, 30.f, 30.f);
    GetMeshRenderer()->SetMesh(CAssetManager::GetInst()->FindAsset<CMesh>(L"Sphere"));
    GetMeshRenderer()->SetMaterial(CAssetManager::GetInst()->FindAsset<CMaterial>(L"Kita"));
    AddComponent(new CRigidBody());

    // AddComponent(new CCollider());      
}

void CFireBall::Update()
{
  /*  Vec3 pos = GetTransform()->GetWorldPosition();
    pos += m_Direction * m_Speed * DELTA_TIME;
    GetTransform()->SetRelativePosition(pos);*/

    CGameObject::Update();
}

void CFireBall::FinalUpdate()
{
    m_ElapsedTime += DELTA_TIME;
    if (m_ElapsedTime >= m_Duration) {
        CLevelManager::GetInst()->GetCurrentLevel()->RemoveGameObject(this);
    }
    CGameObject::FinalUpdate();
}