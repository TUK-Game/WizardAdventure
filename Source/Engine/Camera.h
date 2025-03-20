#pragma once

#include "Component.h"
#include "Frustum.h"

class CCamera :
	public CComponent
{
	friend class CRenderManager;
    friend class CImGuiManager;

public:
	CCamera();
	~CCamera();

public:
    EProjection_Type GetProjType() const    { return m_ProjectionType; }
    ECamera_Type GetCameraType() const { return m_CameraType; }
    float GetOrthoScaleX() const            { return m_OrthoScaleX; }
    float GetAspectRatio() const            { return m_AspectRatio; }
    float GetFar() const                    { return m_Far; }
    float GetFOV() const                    { return (m_FOV / XM_PI) * 180.f; }
    Matrix GetProjMat() const               { return m_matProjection; }
    Matrix GetViewMat() const               { return m_matView; }
    int GetPriority() const                 { return m_Priority; }
    UINT GetLayerCheck()                    { return m_LayerCheck; }
    CGameObject* GetTarget()                { return m_Target; }

    void SetCustomMatView(const Matrix& matView);
    void SetProjType(EProjection_Type type) { m_ProjectionType = type; }
    void SetCameraType(ECamera_Type type)     { m_CameraType = type; }
    void SetOrthoScaleX(float sale)         { m_OrthoScaleX = sale; }
    void SetAspectRatio(float ratio)        { m_AspectRatio = ratio; }
    void SetFOV(float fov)                  { m_FOV = (fov / 180.f) * XM_PI; }
    void SetFar(float _far)                  { m_Far = _far; }
    void SetPriority(int priority);
    void SetWidth(float width)              { m_Width = width; }
    void SetHeight(float height)            { m_Height = height; }
    void SetScale(float scale)              { m_Scale = scale; }
    void SetNear(float value)               { m_Near = value; }
    void SetTarget(CGameObject* target)     { m_Target = target; }

    void CheckLayer(UINT layerIndex)        { m_LayerCheck ^= (1 << layerIndex); }
    void CheckLayerAll()                    { m_LayerCheck = 0xffffffff; }
    void CheckLayerClear()                  { m_LayerCheck = 0; }

public:
	virtual void FinalUpdate() override;
	void Render();
    void RenderDeferred();
    void RenderForward();
    void RenderShadow();
    void SortShadowObject();

private:
    void SortObject();
    void PushLightData();

public:
	virtual CCamera* Clone() override { return new CCamera(*this); }

private:
    EProjection_Type    m_ProjectionType;
    ECamera_Type        m_CameraType = ECamera_Type::Free;
    float               m_OrthoScaleX;  // 직교투영 가로길이
    float               m_AspectRatio;  // 종횡비
    float               m_Near = 1.f;
    float               m_Far = 1000.f;
    float               m_FOV = XM_PI / 4.f;
    float               m_Scale = 1.f;
    float               m_Width = 0.f;
    float               m_Height = 0.f;

    bool                m_bUseCustomMatView = false;

    Matrix              m_CustomMatView;
    Matrix              m_matView;
    Matrix              m_matProjection;

    int                 m_Priority;     // 카메라 우선순위, -1 : 미등록 카메라, 0 : 메인 카메라, 1 ~ : 서브 카메라

    UINT                m_LayerCheck;   // 카메라가 렌더링할 레이어 비트설정

    CFrustum            m_Frustum;

    std::vector<CGameObject*>   m_vecObjects;
    std::vector<CGameObject*>	m_vecDeferred;
    std::vector<CGameObject*>	m_vecForward;
    std::vector<CGameObject*>	m_vecParticle;
    std::vector<CGameObject*>   m_vecShadow;
    
    class CGameObject* m_Target;

public:
    static Matrix s_matView;
    static Matrix s_matProjection;
};

