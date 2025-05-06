#pragma once
#include "Component.h"

class CLight :
    public CComponent
{
public:
    CLight();
    virtual ~CLight();

    virtual void FinalUpdate() override;
	void Render();
	void RenderShadow();

public:
	LIGHT_TYPE GetLightType() { return static_cast<LIGHT_TYPE>(m_Light.lightType); }

	LightInfo& GetLightInfo() { return m_Light; }
	Vec3 GetDiffuse() { return Vec3(m_Light.color.diffuse.x, m_Light.color.diffuse.y, m_Light.color.diffuse.z); }
	Vec3 GetAmbient() { return Vec3(m_Light.color.ambient.x, m_Light.color.ambient.y, m_Light.color.ambient.z); }
	Vec3 GetSpecular() { return Vec3(m_Light.color.specular.x, m_Light.color.specular.y, m_Light.color.specular.z); }
	CGameObject* GetShadowCamera() { return m_ShadowCamera; }

	void SetLightDirection(Vec3 direction);

	void SetDiffuse(const Vec3& diffuse) { m_Light.color.diffuse = diffuse; }
	void SetAmbient(const Vec3& ambient) { m_Light.color.ambient = ambient; }
	void SetSpecular(const Vec3& specular) { m_Light.color.specular = specular; }

	void SetLightType(LIGHT_TYPE type);
	void SetLightRange(float range) { m_Light.range = range; }
	void SetLightAngle(float angle) { m_Light.angle = angle; }

	void SetLightIndex(INT8 index) { m_LightIndex = index; }
public:
	virtual CLight* Clone() override { return new CLight(*this); }

private:
	void UpdateShadowVP();


private:
    LightInfo m_Light = {}; // 조명 데이터

	INT8				m_LightIndex = -1;
	class CMesh*		m_VolumeMesh;
	class CMaterial*	m_LightMaterial;

	CGameObject* m_ShadowCamera;
};