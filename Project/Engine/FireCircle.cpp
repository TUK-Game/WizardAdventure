#include "pch.h"
#include "FireCircle.h"
#include "Transform.h"
#include "MeshRenderer.h"
#include "AssetManager.h"
#include "Level.h"
#include "Layer.h"
#include "LevelManager.h"
#include "Engine.h"

CFireCircle::CFireCircle()
{
    m_type = SKILL::FIRE_CIRCLE;
    AddComponent(new CTransform());
    AddComponent(new CMeshRenderer());  
    GetTransform()->SetRelativeRotation(90.f, 0.f, 0.f);
        
    GetMeshRenderer()->SetMesh(CAssetManager::GetInst()->FindAsset<CMesh>(L"Rectangle"));
    GetMeshRenderer()->SetMaterial(CAssetManager::GetInst()->FindAsset<CMaterial>(L"Circle")->Clone());
}

void CFireCircle::Update()
{
    CSkillObject::Update();
}

void CFireCircle::FinalUpdate()
{
    CGameObject::FinalUpdate();
    if (m_bOwn)
    {
        m_ElapsedTime += DELTA_TIME;
        if (m_ElapsedTime >= m_Duration) {
            m_bDelete = true;
            //CLevelManager::GetInst()->GetCurrentLevel()->GetLayer(GetLayerIndex())->SafeRemoveGameObject(this);
        }
    }
    GetMeshRenderer()->GetMaterial()->SetVec4(0, Vec4(1.f, 0.5f, 0.5f, 1.f));
}