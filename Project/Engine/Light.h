#pragma once
#include "Component.h"

#define MAX_LIGHTS 50

struct LIGHT
{
    XMFLOAT4 m_xmf4Ambient;
    XMFLOAT4 m_xmf4Diffuse;
    XMFLOAT4 m_xmf4Specular;
    XMFLOAT3 m_xmf3Position;
    float    m_fFalloff;
    XMFLOAT3 m_xmf3Direction;
    float    m_fTheta;
    XMFLOAT3 m_xmf3Attenuation;
    float    m_fPhi;
    bool     m_bEnable;
    int      m_nType;  // 0: Directional, 1: Point, 2: Spot
    float    m_fRange;
    float    padding;
};

struct LIGHTS
{
    LIGHT    m_pLights[MAX_LIGHTS];
    XMFLOAT4 m_xmf4GlobalAmbient;
    int      m_nLights;
    float    padding[3]; // 패딩으로 16바이트 정렬 유지
};

class CLight :
    public CComponent
{
public:
    CLight();
    virtual ~CLight();

    virtual void FinalUpdate() override;

public:
    const LIGHT& GetLightData() const { return m_Light; }

    // Setters
    void SetAmbient(const XMFLOAT4& ambient) { m_Light.m_xmf4Ambient = ambient; }
    void SetDiffuse(const XMFLOAT4& diffuse) { m_Light.m_xmf4Diffuse = diffuse; }
    void SetSpecular(const XMFLOAT4& specular) { m_Light.m_xmf4Specular = specular; }
    void SetPosition(const XMFLOAT3& position) { m_Light.m_xmf3Position = position; }
    void SetDirection(const XMFLOAT3& direction) { m_Light.m_xmf3Direction = direction; }
    void SetType(int type) { m_Light.m_nType = type; }
    void SetRange(float range) { m_Light.m_fRange = range; }
    void SetFalloff(float falloff) { m_Light.m_fFalloff = falloff; }
    void SetTheta(float theta) { m_Light.m_fTheta = theta; }
    void SetPhi(float phi) { m_Light.m_fPhi = phi; }
    void SetAttenuation(const XMFLOAT3& attenuation) { m_Light.m_xmf3Attenuation = attenuation; }
    void Enable(bool enable) { m_Light.m_bEnable = enable; }

private:
    LIGHT m_Light; // 조명 데이터
};