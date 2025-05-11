#include "pch.h"
#include "AttackRangeCircle.h"
#include "Transform.h"
#include "MeshRenderer.h"
#include "AssetManager.h"
#include "Engine.h"
#include "LevelManager.h"
#include "Level.h"
#include "Layer.h"

CAttackRangeCircle::CAttackRangeCircle()
{
    AddComponent(new CTransform());
    AddComponent(new CMeshRenderer());

    GetTransform()->SetRelativeRotation(90.f, 0.f, 0.f); // �ٴڿ� ���� ����

    GetMeshRenderer()->SetMesh(CAssetManager::GetInst()->FindAsset<CMesh>(L"Rectangle"));
    GetMeshRenderer()->SetMaterial(CAssetManager::GetInst()->FindAsset<CMaterial>(L"Circle")); // ��� �� �ؽ�ó
}

void CAttackRangeCircle::Update()
{
    CGameObject::Update();
}

void CAttackRangeCircle::FinalUpdate()
{
    CGameObject::FinalUpdate();

    m_ElapsedTime += DELTA_TIME;

    float t = m_ElapsedTime / m_Duration;
    t = std::clamp(t, 0.f, 1.f);

    Vec3 scale = m_StartScale + (m_EndScale - m_StartScale) * t;
    GetTransform()->SetRelativeScale(scale);

    if (m_ElapsedTime >= m_Duration)
    {
        //m_bDelete = true;
        SetEnable(false);
    }

    // ���� tint ȿ��
    GetMeshRenderer()->GetMaterial()->SetVec4(0, Vec4(1.f, 0.1f, 0.1f, 1.f));
}
