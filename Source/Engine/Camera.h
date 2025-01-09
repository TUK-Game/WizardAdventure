#pragma once

#include "Component.h"

class CCamera :
	public CComponent
{
	friend class CRenderManager;

public:
	CCamera();
	~CCamera();

public:
    EProjection_Type GetProjType() const    { return m_ProjectionType; }
    float GetOrthoScaleX() const            { return m_OrthoScaleX; }
    float GetAspectRatio() const            { return m_AspectRatio; }
    float GetFar() const                    { return m_Far; }
    float GetFOV() const                    { return (m_FOV / XM_PI) * 180.f; }
    Matrix GetProjMat() const               { return m_matProjection; }

    void SetProjType(EProjection_Type type) { m_ProjectionType = type; }
    void SetOrthoScaleX(float sale)         { m_OrthoScaleX = sale; }
    void SetAspectRatio(float ratio)        { m_AspectRatio = ratio; }
    void SetFOV(float fov)                  { m_FOV = (fov / 180.f) * XM_PI; }
    void SetFar(float _far)                  { m_Far = _far; }
    void SetPriority(int priority);

    void CheckLayer(UINT layerIndex)        { m_LayerCheck ^= (1 << layerIndex); }
    void CheckLayerAll()                    { m_LayerCheck = 0xffffffff; }
    void CheckLayerClear()                  { m_LayerCheck = 0; }

public:
	virtual void FinalUpdate() override;
	void Render();

private:
    void SortObject();

public:
	virtual CCamera* Clone() override { return new CCamera(*this); }

private:
    EProjection_Type    m_ProjectionType;
    float               m_OrthoScaleX;  // 직교투영 가로길이
    float               m_AspectRatio;  // 종횡비
    float               m_FOV;          // 시야각(FieldOfView)
    float               m_Far;          // 최대 시야거리

    Matrix              m_matView;
    Matrix              m_matProjection;

    int                 m_Priority;     // 카메라 우선순위, -1 : 미등록 카메라, 0 : 메인 카메라, 1 ~ : 서브 카메라

    UINT                m_LayerCheck;   // 카메라가 렌더링할 레이어 비트설정

    std::vector<CGameObject*>   m_vecObjects;

public:
    static Matrix s_matView;
    static Matrix s_matProjection;
};

