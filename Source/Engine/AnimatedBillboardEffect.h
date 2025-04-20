#pragma once
#include "GameObject.h"

struct BillboardEffectDesc
{
    std::wstring textureKey;
    int spriteX = 1;
    int spriteY = 1;
    float framePerSecond = 10.f;
    bool loop = false;
    bool scaleOverTime = false;
    float startScale = 1.0f;
    float endScale = 2.0f;
    float startAlpha = 1.f;
    float endAlpha = 1.f;
    bool useRigidMove = false;
    Vec3 initialVelocity = Vec3(0.f, 0.f, 0.f); // 속도 직접 설정
    bool useGravity = false;
};

class CAnimatedBillboardEffect :
    public CGameObject
{
public:
    CAnimatedBillboardEffect();
    virtual ~CAnimatedBillboardEffect();

    void Init(const BillboardEffectDesc& desc);
    void Reset();
    void SetDesc(const BillboardEffectDesc& desc) { m_Desc = desc; m_TotalFrames = m_Desc.spriteX * m_Desc.spriteY; }

    virtual void Update() override;
    virtual void FinalUpdate() override;

private:
    BillboardEffectDesc m_Desc;
    float m_AccTime = 0.f;
    int m_CurrentFrame = 0;
    int m_TotalFrames = 0;
};

