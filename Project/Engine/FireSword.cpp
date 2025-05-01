#include "pch.h"
#include "FireSword.h"
#include "Transform.h"
#include "MeshRenderer.h"
#include "MeshData.h"
#include "RigidBody.h"
#include "AssetManager.h"
#include "Level.h"
#include "Layer.h"
#include "LevelManager.h"
#include "Engine.h"

CFireSword::CFireSword()
{
    m_type = SKILL::FIRE_SWORD;
    AddComponent(new CTransform());
    CMeshData* data2 = CAssetManager::GetInst()->FindAsset<CMeshData>(L"SwordLava");
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

void CFireSword::Update()
{
    if (m_bOwn)
    {
        Vec3 pos = GetTransform()->GetRelativePosition();

        if (!m_ReadyToRotate)
        {
            m_Elapsed += DELTA_TIME;
            float t = m_Elapsed / m_TranslateWaitTime;
            t = std::clamp(t, 0.0f, 1.0f);

            for (auto obj : GetChild())
            {
                obj->GetTransform()->SetRelativeScale(t, t, t);
            }

            float readySpeed = m_Speed * ((1.0f - t) / 1.0f);
            pos += m_ReadyDirection * readySpeed * DELTA_TIME;
            GetTransform()->SetRelativePosition(pos);

            if (m_Elapsed >= m_RotateWaitTime)
            {
                m_ReadyToRotate = true;
                m_Elapsed = 0.f;
                Vec3 targetDir = m_TargetPos - pos;
                targetDir.Normalize();
                m_Direction = targetDir;
            }
            return;
        }

        if (m_ReadyToRotate && !m_ReadyToFire)
        {
            m_Elapsed += DELTA_TIME;
            float t = m_Elapsed / m_RotateWaitTime;
            t = std::clamp(t, 0.0f, 1.0f);

            // 보간된 방향o
            Vec3 blendedDirection = DirectX::SimpleMath::Vector3::Lerp(m_ReadyDirection, m_Direction, t);
            blendedDirection.Normalize();
            GetTransform()->LookAt(blendedDirection);

            if (m_Elapsed >= m_RotateWaitTime)
            {
                m_ReadyToFire = true;
                m_Elapsed = 0.f;
            }
            return;
        }

        pos += m_Direction * m_Speed * DELTA_TIME;
        GetTransform()->SetRelativePosition(pos);
    }
    CSkillObject::Update();
}

void CFireSword::FinalUpdate()
{
    CGameObject::FinalUpdate();
    if (m_bOwn)
    {
        Vec3 pos = GetTransform()->GetRelativePosition();
        if (pos.y < -200.f) // 충돌시 삭제로 변경해야함
        {
            m_bDelete = true;
            //CLevelManager::GetInst()->GetCurrentLevel()->GetLayer(GetLayerIndex())->SafeRemoveGameObject(this);
        }
    }
}