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
public:
	const LightInfo& GetLightInfo() { return m_Light; }

	void SetLightDirection(const Vec3& direction) { m_Light.direction = direction; }

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
    LightInfo m_Light = {}; // ���� ������

	INT8				m_LightIndex = -1;
	class CMesh*		m_VolumeMesh;
	class CMaterial*	m_LightMaterial;
};