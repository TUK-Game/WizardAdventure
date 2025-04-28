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
	void SetEmit(bool value) { m_bEmit = value; }
	bool IsEmitting() const { return m_bEmit; }
	bool IsAvailable() const { return m_bAvailable; }
	void SetAvailable(bool value) { m_bAvailable = value; }

	void ExplodeAt(const Vec3& pos);

public:

	virtual void FinalUpdate();
	void Render();

public:
	virtual CParticleSystem* Clone() override { return new CParticleSystem(*this); }

private:
	std::shared_ptr<CStructuredBuffer>	m_ParticleBuffer;
	std::shared_ptr<CStructuredBuffer>	m_ComputeSharedBuffer;	
	UINT32							m_MaxParticle = 100;

	CMaterial*						m_ComputeMaterial;
	CMaterial*						m_GraphicsMaterial;
	CMesh*							m_Mesh;


	Vec3				m_BasePos{};
	float				m_CreateInterval = 0.005f;
	float				m_AccTime = 0.f;

	float				m_MinLifeTime = 0.5f;
	float				m_MaxLifeTime = 1.f;
	float				m_MinSpeed = 100;
	float				m_MaxSpeed = 200;
	float				m_StartScale = 50.f;
	float				m_EndScale = 5.f;

	bool				m_bEmit = true;
	bool				m_bExplosionMode = false;
	bool				m_bAvailable = true;

	float				m_ExplosionDuration = 0.5f; 
	float				m_ExplosionElapsed = 0.f;  
};
