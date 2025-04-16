#pragma once
#include "Component.h"

class CMaterial;
class CMesh;
class CStructuredBuffer;

struct ParticleInfo
{
	Vec3	worldPos;
	float	curTime;
	Vec3	worldDir;
	float	lifeTime;
	INT32	alive;
	INT32	padding[3];
};

struct ComputeSharedInfo
{
	INT32 addCount;
	INT32 padding[3];
};

class CParticleSystem : public CComponent
{
public:
	CParticleSystem();
	CParticleSystem(UINT32 maxParticle);
	virtual ~CParticleSystem();

public:
	void SetGarpchisMaterial(CMaterial* material) { m_GraphicsMaterial = material; }
	void SetComputeMaterial(CMaterial* material) { m_ComputeMaterial = material; }
	void SetTexture(class CTexture* texture);
	void SetTexture(const std::wstring& name);
	void SetBasePos(const Vec3 pos) { m_BasePos = pos; }
public:

	virtual void FinalUpdate();
	void Render();

public:
	virtual CParticleSystem* Clone() override { return new CParticleSystem(*this); }

private:
	std::shared_ptr<CStructuredBuffer>	m_ParticleBuffer;
	std::shared_ptr<CStructuredBuffer>	m_ComputeSharedBuffer;	
	UINT32							m_MaxParticle = 1000;

	CMaterial*						m_ComputeMaterial;
	CMaterial*						m_GraphicsMaterial;
	CMesh*							m_Mesh;


	Vec3				m_BasePos{};
	float				_createInterval = 0.005f;
	float				m_AccTime = 0.f;

	float				_minLifeTime = 0.5f;
	float				_maxLifeTime = 1.f;
	float				_minSpeed = 100;
	float				_maxSpeed = 50;
	float				_startScale = 10.f;
	float				_endScale = 5.f;
};
