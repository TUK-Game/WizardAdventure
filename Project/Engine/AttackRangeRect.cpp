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

    // ������ ������Ʈ
    Vec3 scale = m_StartScale + (m_EndScale - m_StartScale) * t;
    GetTransform()->SetRelativeScale(scale);

    // ���� �������� ���ݸ�ŭ �̵�
    Vec3 direction = -GetTransform()->GetWorldDir(EDir::Up); // �չ���
    float offset = scale.y * 0.5f;          // ���ݸ�ŭ ������
    Vec3 basePos = m_InitialPosition + direction * offset;
    GetTransform()->SetRelativePosition(basePos);


    if (m_ElapsedTime >= m_Duration)
    {
        CLevelManager::GetInst()->GetCurrentLevel()->GetLayer(GetLayerIndex())->SafeRemoveGameObject(this);
    }

    GetMeshRenderer()->GetMaterial()->SetVec4(0, Vec4(1.f, 0.f, 0.f, 1.f)); // ������ tint
}