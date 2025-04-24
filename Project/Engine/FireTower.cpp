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
#include "EffectManager.h"

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
        if (!m_bFinishScale) 
            UpdateScaleLerp();
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
            if (m_FireEffect) {
                m_FireEffect->SetEnable(false);
                m_FireEffect = nullptr; 
            }
            m_bDelete = true;
        }
    }
}

void CFireTower::UpdateScaleLerp()
{
    if (m_ElapsedTime >= m_ScaleDuration) {
        m_bFinishScale = true;
        auto pos = GetTransform()->GetRelativePosition();
        pos += Vec3(0.f, 250.f, 0.f);
        m_FireEffect = CEffectManager::GetInst()->SpawnEffect(L"fire", pos);
        return;
    }
    float t = m_ElapsedTime / m_ScaleDuration;
    t = std::clamp(t, 0.f, 1.f);
    GetTransform()->SetRelativeScale(1.f, t, 1.f);
}