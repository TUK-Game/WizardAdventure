#include "pch.h"
#include "AnimatedBillboardEffect.h"
#include "AssetManager.h"
#include "Material.h"
#include "MeshRenderer.h"
#include "Transform.h"
#include "RigidBody.h"
#include "Engine.h"

CAnimatedBillboardEffect::CAnimatedBillboardEffect()
{
    AddComponent(new CTransform());
    AddComponent(new CMeshRenderer());
    AddComponent(new CRigidBody());
}

CAnimatedBillboardEffect::~CAnimatedBillboardEffect()
{
}

void CAnimatedBillboardEffect::Init(const BillboardEffectDesc& desc)
{
    m_Desc = desc;
    m_TotalFrames = m_Desc.spriteX * m_Desc.spriteY;

    auto mesh = CAssetManager::GetInst()->FindAsset<CMesh>(L"Point");
    auto material = new CMaterial;
    material->SetTexture(0, CAssetManager::GetInst()->FindAsset<CTexture>(desc.textureKey));
    material->SetGraphicsShader(CAssetManager::GetInst()->FindAsset<CGraphicShader>(L"BillboardAnimated"));

    material->SetInt(1, desc.spriteX);  // int_1
    material->SetInt(2, desc.spriteY);  // int_2
    material->SetInt(3, 0);             // int_3: frame index

    material->SetFloat(0, desc.startScale);           // float_0
    material->SetFloat(1, desc.endScale);             // float_1
    material->SetFloat(2, 0.f);                       // float_2: current frame (for scale)
    material->SetFloat(3, static_cast<float>(m_TotalFrames)); // float_3: total frames
    material->SetVec2(2, Vec2(desc.startAlpha, desc.endAlpha)); // vec2_2


    GetMeshRenderer()->SetMesh(mesh);
    GetMeshRenderer()->SetMaterial(material);
}

void CAnimatedBillboardEffect::Update()
{
    CGameObject::Update();

    m_AccTime += DELTA_TIME;
    int frame = static_cast<int>(m_AccTime * m_Desc.framePerSecond);

    if (!m_Desc.loop && frame >= m_TotalFrames)
    {
        SetEnable(false); // 자동 소멸
        return;
    }

    m_CurrentFrame = frame % m_TotalFrames;

    auto mat = GetMeshRenderer()->GetMaterial();
    mat->SetInt(3, m_CurrentFrame); // int_3 = frame index
    mat->SetFloat(2, static_cast<float>(m_CurrentFrame)); // float_2 = scale 계산용 frame index
}

void CAnimatedBillboardEffect::FinalUpdate()
{
    CGameObject::FinalUpdate();
}

void CAnimatedBillboardEffect::Reset()
{
    m_AccTime = 0.f;
    m_CurrentFrame = 0;

    auto mat = GetMeshRenderer()->GetMaterial();
    mat->SetInt(3, 0); // int_3 = frame index
    mat->SetFloat(2, 0.f); // float_2 = current frame (for scale)

    CRigidBody* rigid = GetRigidBody();
    if (rigid && m_Desc.useRigidMove)
    {
        rigid->SetKinematic(true); // 움직임 허용
        rigid->SetVelocity(m_Desc.initialVelocity);
        rigid->SetGravity(m_Desc.useGravity);
    }
}