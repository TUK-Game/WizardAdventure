#include "pch.h"
#include "FirePillar.h"
#include "Transform.h"
#include "MeshRenderer.h"
#include "RigidBody.h"
#include "AssetManager.h"
#include "Level.h"
#include "Layer.h"
#include "LevelManager.h"
#include "Engine.h"

CFirePillar::CFirePillar()
{
    AddComponent(new CTransform());
    AddComponent(new CMeshRenderer());  
    GetTransform()->SetRelativeScale(70.f, 300.f, 70.f);
    GetMeshRenderer()->SetMesh(CAssetManager::GetInst()->FindAsset<CMesh>(L"Cube"));        // 기둥으로 교체해야함
    GetMeshRenderer()->SetMaterial(CAssetManager::GetInst()->FindAsset<CMaterial>(L"Lava"));
    AddComponent(new CRigidBody());
    CRigidBody* rb = GetRigidBody();

    float speed = RandomFloat(100.f, 200.f);
    rb->SetVelocity(Vec3(0.f, speed, 0.f)); 
    m_ShakeFreqX = RandomFloat(10.f, 15.f);
    m_ShakeFreqZ = RandomFloat(10.f, 15.f);
    m_PhaseOffsetX = RandomFloat(0.f, XM_2PI);
    m_PhaseOffsetZ = RandomFloat(0.f, XM_2PI);
    // AddComponent(new CCollider());      
}

void CFirePillar::Update()
{
    if (m_ElapsedTime > 3.f) {
        GetRigidBody()->SetKinematic(false);
    }
    else {
        Vec3 pos = GetTransform()->GetRelativePosition();
        m_BasePos = pos;
        float offsetX = sinf(m_ElapsedTime * m_ShakeFreqX + m_PhaseOffsetX) * m_ShakeAmount;
        float offsetZ = cosf(m_ElapsedTime * m_ShakeFreqZ + m_PhaseOffsetZ) * m_ShakeAmount;
        Vec3 basePos = m_BasePos;
        basePos.x += offsetX;
        basePos.z += offsetZ;
        GetTransform()->SetRelativePosition(basePos);
    }

    CGameObject::Update();
}

void CFirePillar::FinalUpdate()
{
    CGameObject::FinalUpdate();
    m_ElapsedTime += DELTA_TIME;
    if (m_ElapsedTime >= m_Duration) {
        CLevelManager::GetInst()->GetCurrentLevel()->GetLayer(GetLayerIndex())->SafeRemoveGameObject(this);
    }
}