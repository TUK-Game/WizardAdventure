#pragma once

class CLevel;
class CGameObject;

class CParticleSystemManager
	: public CSingleton<CParticleSystemManager>
{
	DECLARE_SINGLETON(CParticleSystemManager)

public:

	void Init(int poolSize, CLevel* level);
	CGameObject* Request();
	void Return(CGameObject* ps);
	void Clear();
private:
	std::vector<CGameObject*> m_Pool;
};

