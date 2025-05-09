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

    SetInstancing(false);
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

            int diff = stats->currentHp - stats->displayHp;

            if (diff != 0)
            {
                if (-10 < diff && diff < 0)
                {
                    stats->displayHp--;
                }
                else if (0 < diff && diff < 10)
                {
                    stats->displayHp++;
                }
                else
                {
                    stats->displayHp += static_cast<int>(static_cast<float>(diff) / 10.f);
                }
            }


            m_HpRatio = static_cast<float>(stats->displayHp) / static_cast<float>(stats->maxHp);
            m_HpRatio = std::clamp(m_HpRatio, 0.f, 1.f);
        }
    }
}

void CMonsterHPBar::SetBeforeRenderPushParmater()
{
    GetMeshRenderer()->GetMaterial()->SetFloat(0, m_HpRatio);
}
