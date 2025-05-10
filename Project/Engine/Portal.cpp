#include "pch.h"
#include "Portal.h"
#include "MeshRenderer.h"
#include "Transform.h"
#include "AssetManager.h"
#include "Engine.h"
#include "ParticleSystem.h"
#include "ParticleSystemManager.h"

CPortal::CPortal()
   : m_RotationSpeed(90.f) 
{
    SetName(L"Portal");

    AddComponent(new CTransform());
    AddComponent(new CMeshRenderer());
    GetTransform()->SetRelativeScale(300.f, 200.f, 300.f);
    GetMeshRenderer()->SetMesh(CAssetManager::GetInst()->FindAsset<CMesh>(L"HollowCylinder"));
    GetMeshRenderer()->SetMaterial(CAssetManager::GetInst()->FindAsset<CMaterial>(L"Mask1"));

    m_Floor = new CGameObject;
    m_Floor->SetName(L"Portal_Floor");
    m_Floor->AddComponent(new CTransform());
    m_Floor->AddComponent(new CMeshRenderer());
    m_Floor->GetTransform()->SetRelativeScale(1.4f, 1.4f, 1.4f);
    m_Floor->GetTransform()->SetRelativeRotation(90.f, 0.f, 0.f); // 바닥에 누운 형태
    m_Floor->GetMeshRenderer()->SetMesh(CAssetManager::GetInst()->FindAsset<CMesh>(L"Rectangle"));
    m_Floor->GetMeshRenderer()->SetMaterial(CAssetManager::GetInst()->FindAsset<CMaterial>(L"MagicCircle"));
    AddChild(m_Floor);
    m_PortalParticleId = CParticleSystemManager::GetInst()->AddEmitter(L"Portal", GetTransform()->GetRelativePosition());

}

CPortal::~CPortal()
{
    if (0 <= m_PortalParticleId)
    {
        CParticleSystemManager::GetInst()->RemoveEmitter(L"Portal", m_PortalParticleId);
    }
}

void CPortal::Update()
{
    CGameObject::Update(); 

    Vec3 rot = GetTransform()->GetRelativeRotation(); 
    rot.y += m_RotationSpeed * DELTA_TIME; 
    if (rot.y >= 360.f)
        rot.y -= 360.f;

    GetTransform()->SetRelativeRotation(rot);

    // tint
    m_Floor->GetMeshRenderer()->GetMaterial()->SetVec4(0, tint);
    GetMeshRenderer()->GetMaterial()->SetVec4(0, tint);


    Vec3 pos = GetTransform()->GetRelativePosition();

    if (0 <= m_PortalParticleId)
        CParticleSystemManager::GetInst()->UpdateEmitterPos(L"Portal", m_PortalParticleId, pos);
}