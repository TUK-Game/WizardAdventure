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
    AddComponent(new CTransform());
    AddComponent(new CMeshRenderer());  
    GetTransform()->SetRelativeScale(500.f, 500.f, 200.f);
    GetTransform()->SetRelativeRotation(-90.f, 0.f, 0.f);

    GetMeshRenderer()->SetMesh(CAssetManager::GetInst()->FindAsset<CMesh>(L"Circle"));
    GetMeshRenderer()->SetMaterial(CAssetManager::GetInst()->FindAsset<CMaterial>(L"Lava"));
}

void CFireCircle::Update()
{
    CGameObject::Update();
}

void CFireCircle::FinalUpdate()
{
    CGameObject::FinalUpdate();
    m_ElapsedTime += DELTA_TIME;
    if (m_ElapsedTime >= m_Duration) {
        CLevelManager::GetInst()->GetCurrentLevel()->GetLayer(GetLayerIndex())->SafeRemoveGameObject(this);
    }
}