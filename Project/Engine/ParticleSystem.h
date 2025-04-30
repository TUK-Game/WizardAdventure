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
	int	alive;
	int emitterID;
	int	padding[3];
};

struct EmitterInfo
{
	Vec3 basePos;
	float isAlive; 
};

struct ComputeSharedInfo
{
	int addCount;
	float padding[3];
};

class CParticleSystem : public CComponent
{
public:
	CParticleSystem();
	CParticleSystem(UINT32 maxParticle);
	virtual ~CParticleSystem();

public:
	void SetGraphicsMaterial(CMaterial* material) { m_GraphicsMaterial = material; }
	void SetComputeMaterial(CMaterial* material) { m_ComputeMaterial = material; }
	void SetTexture(class CTexture* texture);
	void SetTexture(const std::wstring& name);
	void SetEmit(bool value) { m_bEmit = value; }
	bool IsEmitting() const { return m_bEmit; }


	int AddEmitter(const Vec3& basePos);
	void UpdateEmitterPos(int emitterID, const Vec3& newPos);
	void RemoveEmitter(int emitterID);
	void ClearEmitters();

public:

	virtual void FinalUpdate();
	void Render();

public:
	virtual CParticleSystem* Clone() override { return new CParticleSystem(*this); }

private:
	void InitBuffers(UINT32 maxParticle, UINT32 maxEmitter);

private:
	std::shared_ptr<CStructuredBuffer>	m_ParticleBuffer;
	std::shared_ptr<CStructuredBuffer>	m_EmitterBuffer;
	std::shared_ptr<CStructuredBuffer>	m_ComputeSharedBuffer;	
	UINT32							m_MaxParticle = 1000;
	UINT32							m_MaxEmitter = 50;

	std::vector<EmitterInfo>		m_Emitters;

	CMaterial*						m_ComputeMaterial;
	CMaterial*						m_GraphicsMaterial;
	CMesh*							m_Mesh;


	float				m_CreateInterval = 0.005f;
	float				m_AccTime = 0.f;

	float				m_MinLifeTime = 0.5f;
	float				m_MaxLifeTime = 1.f;
	float				m_MinSpeed = 100;
	float				m_MaxSpeed = 200;
	float				m_StartScale = 50.f;
	float				m_EndScale = 5.f;

	bool				m_bEmit = true;

};
