#pragma once

class CLevel;
class CGameObject;

struct ReturnRequest
{
	CGameObject* obj;
	float remainingTime;
};

class CParticleSystemManager
	: public CSingleton<CParticleSystemManager>
{
	DECLARE_SINGLETON(CParticleSystemManager)

public:

	void Init(int poolSize, CLevel* level);
	void Update(float deltaTime);
	CGameObject* Request();
	void Return(CGameObject* ps);
	void Clear();
private:
	std::vector<CGameObject*> m_Pool;
	std::vector<ReturnRequest> m_ReturnQueue;

};

