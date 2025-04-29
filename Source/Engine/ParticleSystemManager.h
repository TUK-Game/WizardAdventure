#pragma once

class CLevel;
class CGameObject;
class CParticleSystem;

class CParticleSystemManager
	: public CSingleton<CParticleSystemManager>
{
	DECLARE_SINGLETON(CParticleSystemManager)

public:
    void Init(CLevel* level);
    int AddEmitter(const std::wstring& name, const Vec3& pos);
    void UpdateEmitterPos(const std::wstring& name, int emitterID, const Vec3& newPos);
    void RemoveEmitter(const std::wstring& name, int emitterID);
    void Update(float deltaTime);
    void Clear();

private:
    std::unordered_map<std::wstring, CGameObject*> m_ParticleObjects;

    CParticleSystem* m_ParticleSystem = nullptr;
    CGameObject* m_ParticleObject = nullptr;

};

