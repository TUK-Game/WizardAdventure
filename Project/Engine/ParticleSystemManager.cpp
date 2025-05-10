#include "pch.h"
#include "ParticleSystemManager.h"
#include "ParticleSystem.h"
#include "GameObject.h"
#include "Level.h"
#include "Transform.h"
#include "AssetManager.h"

CParticleSystemManager::CParticleSystemManager()
{
}

CParticleSystemManager::~CParticleSystemManager()
{
}

void CParticleSystemManager::Init(CLevel* level)
{
    // smoke
    CGameObject* obj = new CGameObject();
    CParticleSystem* ps = new CParticleSystem();

    ps->SetTexture(L"Smoke");

    obj->AddComponent(ps);
    obj->AddComponent(new CTransform());
    obj->SetName(L"SmokeParticle"); 

    level->AddGameObject(obj, LAYER_EFFECT, false);

    m_ParticleObjects.insert({ L"Smoke", obj});

    // spark
    obj = new CGameObject();
    ps = new CParticleSystem();

    obj->AddComponent(ps);
    obj->AddComponent(new CTransform());
    obj->SetName(L"SparkParticle");

    level->AddGameObject(obj, LAYER_EFFECT, false);

    m_ParticleObjects.insert({L"Spark", obj });

    // light
    obj = new CGameObject();
    ps = new CParticleSystem();

    ps->SetTexture(L"Light");
    ps->SetCreateInterval(0.02f);
    obj->AddComponent(ps);
    obj->AddComponent(new CTransform());
    obj->SetName(L"LightParticle");

    level->AddGameObject(obj, LAYER_EFFECT, false);

    m_ParticleObjects.insert({ L"Light", obj });

    // portal
    obj = new CGameObject();
    ps = new CParticleSystem();

    ps->SetTexture(L"BlueLight");
    ps->SetCreateInterval(0.1f);
    ps->SetComputeMaterial(CAssetManager::GetInst()->FindAsset<CMaterial>(L"ComputePortalParticle"));
    obj->AddComponent(ps);
    obj->AddComponent(new CTransform());
    obj->SetName(L"PortalParticle");

    level->AddGameObject(obj, LAYER_EFFECT, false);

    m_ParticleObjects.insert({ L"Portal", obj });

}

int CParticleSystemManager::AddEmitter(const std::wstring& name, const Vec3& pos)
{
    auto it = m_ParticleObjects.find(name);
    if (it == m_ParticleObjects.end())
        return -1;

    CGameObject* obj = it->second;
    CParticleSystem* ps = obj->GetParticleSystem();
    if (!ps)
        return -1;

    return ps->AddEmitter(pos);
}

void CParticleSystemManager::UpdateEmitterPos(const std::wstring& name, int emitterID, const Vec3& newPos)
{
    auto it = m_ParticleObjects.find(name);
    if (it == m_ParticleObjects.end())
        return;

    CGameObject* obj = it->second;
    CParticleSystem* ps = obj->GetParticleSystem();
    if (!ps)
        return;

    ps->UpdateEmitterPos(emitterID, newPos);
}

void CParticleSystemManager::RemoveEmitter(const std::wstring& name, int emitterID)
{
    auto it = m_ParticleObjects.find(name);
    if (it == m_ParticleObjects.end())
        return;

    CGameObject* obj = it->second;
    CParticleSystem* ps = obj->GetParticleSystem();
    if (!ps)
        return;

    ps->RemoveEmitter(emitterID);
}

void CParticleSystemManager::Update(float deltaTime)
{
   
}

void CParticleSystemManager::Clear()
{
    for (auto& [name, obj] : m_ParticleObjects)
    {
        if (obj)
            delete obj;
    }
    m_ParticleObjects.clear();
}
