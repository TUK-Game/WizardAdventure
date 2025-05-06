#include "pch.h"
#include "MonsterHPBar.h"
#include "Transform.h"
#include "MeshRenderer.h"
#include "AssetManager.h"
#include "RenderManager.h"
#include "Camera.h"
#include "Monster.h"

CMonsterHPBar::CMonsterHPBar()
{
    AddComponent(new CTransform());
    AddComponent(new CMeshRenderer());

    GetMeshRenderer()->SetMesh(CAssetManager::GetInst()->FindAsset<CMesh>(L"Point"));
    GetMeshRenderer()->SetMaterial(CAssetManager::GetInst()->FindAsset<CMaterial>(L"HPBar"));
}

void CMonsterHPBar::SetOffsetY(float offsetY)
{
    GetTransform()->SetRelativePosition(0.f, offsetY, 0.f);
}


void CMonsterHPBar::FinalUpdate()
{
    CGameObject::FinalUpdate();

    CGameObject* parent = GetParent();
    if (parent)
    {
        // 체력 비율 적용
        CMonster* monster = dynamic_cast<CMonster*>(parent);
        if (monster)
        {
            auto stats = monster->GetStat();
            float hpRatio = static_cast<float>(stats->currentHp) / static_cast<float>(stats->maxHp);
            hpRatio = std::clamp(hpRatio, 0.f, 1.f);
            GetMeshRenderer()->GetMaterial()->SetFloat(0, hpRatio);
        }
    }
}