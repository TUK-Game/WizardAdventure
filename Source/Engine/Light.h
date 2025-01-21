#pragma once
#include "Component.h"

class CLight :
    public CComponent
{
public:
    CLight();
    virtual ~CLight();

    virtual void FinalUpdate() override;

public:
	const LightInfo& GetLightInfo() { return m_Light; }

	void SetLightDirection(const Vec3& direction) { m_Light.direction = direction; }

	void SetDiffuse(const Vec3& diffuse) { m_Light.color.diffuse = diffuse; }
	void SetAmbient(const Vec3& ambient) { m_Light.color.ambient = ambient; }
	void SetSpecular(const Vec3& specular) { m_Light.color.specular = specular; }

	void SetLightType(LIGHT_TYPE type) { m_Light.lightType = static_cast<INT32>(type); }
	void SetLightRange(float range) { m_Light.range = range; }
	void SetLightAngle(float angle) { m_Light.angle = angle; }

public:
	virtual CLight* Clone() override { return new CLight(*this); }

private:
    LightInfo m_Light = {}; // 조명 데이터
};