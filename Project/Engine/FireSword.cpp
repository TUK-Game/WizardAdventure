#include "pch.h"
#include "FireSword.h"
#include "Transform.h"
#include "MeshRenderer.h"
#include "MeshData.h"
#include "RigidBody.h"
#include "AssetManager.h"
#include "Level.h"
#include "LevelManager.h"
#include "Engine.h"

CFireSword::CFireSword()
{
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
    CGameObject::Update();
    Vec3 pos = GetTransform()->GetWorldPosition();

    if (!m_ReadyToRotate)
    {
        m_Elapsed += DELTA_TIME;
        float t = m_Elapsed / m_TranslateWaitTime;
        t = std::clamp(t, 0.0f, 1.0f);

        float scale = m_ReadyScale * t;
        GetTransform()->SetRelativeScale(scale , scale , scale);

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

        // 보간된 방향
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

    if (m_ReadyToFire)
    {
        pos += m_Direction * m_Speed * DELTA_TIME;
        GetTransform()->SetRelativePosition(pos);
    }
}

void CFireSword::FinalUpdate()
{
    CGameObject::FinalUpdate();
}