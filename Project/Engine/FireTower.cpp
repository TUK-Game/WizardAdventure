#include "pch.h"
#include "FireTower.h"
#include "Transform.h"
#include "MeshRenderer.h"
#include "RigidBody.h"
#include "AssetManager.h"
#include "Level.h"
#include "Layer.h"
#include "LevelManager.h"
#include "Engine.h"
#include "MeshData.h"

CFireTower::CFireTower()
{
    m_type = SKILL::FIRE_PILLAR;
    AddComponent(new CTransform());
    CMeshData* data2 = CAssetManager::GetInst()->FindAsset<CMeshData>(L"FireTower");
    std::vector<CGameObject*> obj2 = data2->Instantiate(ECollision_Channel::Player); // temp
    for (auto& o : obj2)
    {
        std::wstring name = o->GetMeshRenderer()->GetMesh()->GetName();
        o->SetName(name);
        Vec3 rot = o->GetTransform()->GetRelativeRotation();
        o->GetTransform()->SetRelativeRotation(rot);
        //o->GetTransform()->SetRelativeScale(0.2f, 0.2f, 0.2f);
        o->SetCheckFrustum(true);
        o->SetInstancing(false);
        AddChild(o);
    }
    // AddComponent(new CCollider());      
}

void CFireTower::Update()
{
    if (m_bOwn)
    {
  /*      if (m_ElapsedTime > 3.f) {
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
        }*/
    }
    CSkillObject::Update();
}

void CFireTower::FinalUpdate()
{
    CGameObject::FinalUpdate();
    if (m_bOwn)
    {
        m_ElapsedTime += DELTA_TIME;
        if (m_ElapsedTime >= m_Duration) {
            m_bDelete = true;
        }
    }
}