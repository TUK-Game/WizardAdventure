#include "pch.h"
#include "AttackRangeRect.h"
#include "Transform.h"
#include "MeshRenderer.h"
#include "AssetManager.h"
#include "Engine.h"
#include "LevelManager.h"
#include "Level.h"
#include "Layer.h"

CAttackRangeRect::CAttackRangeRect()
{
    AddComponent(new CTransform());
    AddComponent(new CMeshRenderer());

    GetTransform()->SetRelativeRotation(90.f, 0.f, 0.f); 

    GetMeshRenderer()->SetMesh(CAssetManager::GetInst()->FindAsset<CMesh>(L"Rectangle"));
    GetMeshRenderer()->SetMaterial(CAssetManager::GetInst()->FindAsset<CMaterial>(L"RectWarning")); 
}

void CAttackRangeRect::Update()
{
    CGameObject::Update();
}

void CAttackRangeRect::FinalUpdate()
{
    CGameObject::FinalUpdate();

    m_ElapsedTime += DELTA_TIME;

    float t = m_ElapsedTime / m_Duration;
    t = std::clamp(t, 0.f, 1.f);

    // 스케일 업데이트
    Vec3 scale = m_StartScale + (m_EndScale - m_StartScale) * t;
    GetTransform()->SetRelativeScale(scale);

    // 몬스터 정면으로 절반만큼 이동
    Vec3 direction = -GetTransform()->GetWorldDir(EDir::Up); // 앞방향
    float offset = scale.y * 0.5f;          // 절반만큼 앞으로
    Vec3 basePos = m_InitialPosition + direction * offset;
    GetTransform()->SetRelativePosition(basePos);


    if (m_ElapsedTime >= m_Duration)
    {
        CLevelManager::GetInst()->GetCurrentLevel()->GetLayer(GetLayerIndex())->SafeRemoveGameObject(this);
    }

    GetMeshRenderer()->GetMaterial()->SetVec4(0, Vec4(1.f, 0.f, 0.f, 1.f)); // 붉은색 tint
}