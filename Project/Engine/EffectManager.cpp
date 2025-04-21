#include "pch.h"
#include "EffectManager.h"
#include "Transform.h"
#include "LevelManager.h"
#include "Level.h"

CEffectManager::CEffectManager()
{
}

CEffectManager::~CEffectManager()
{
}

void CEffectManager::Init()
{
    BillboardEffectDesc explosion;
    explosion.textureKey = L"explosionSheet";
    explosion.spriteX = 7;
    explosion.spriteY = 7;
    explosion.framePerSecond = 49.f;
    explosion.loop = false;
    explosion.scaleOverTime = true;
    explosion.startScale = 80.f;
    explosion.endScale = 250.f;
    explosion.startAlpha = 1.f;
    explosion.endAlpha = 0.5f;
    RegisterEffectTemplate(L"Explosion", explosion);

    BillboardEffectDesc explosion1;
    explosion1.textureKey = L"explosionSheet1";
    explosion1.spriteX = 8;
    explosion1.spriteY = 8;
    explosion1.framePerSecond = 96.f;
    explosion1.loop = false;
    explosion1.scaleOverTime = true;
    explosion1.startScale = 80.f;
    explosion1.endScale = 80.f;
    explosion1.startAlpha = 1.f;
    explosion1.endAlpha = 0.5f;
    RegisterEffectTemplate(L"Explosion1", explosion1);

    BillboardEffectDesc smoke;
    smoke.textureKey = L"SmokeSheet";
    smoke.spriteX = 8;
    smoke.spriteY = 8;
    smoke.framePerSecond = 64.f;
    smoke.loop = false;
    smoke.scaleOverTime = true;
    smoke.startScale = 50.f;
    smoke.endScale = 100.f;
    smoke.startAlpha = 1.f;
    smoke.endAlpha = 0.f;
    RegisterEffectTemplate(L"Smoke", smoke);

    BillboardEffectDesc shockwave;
    shockwave.textureKey = L"ShockwaveSheet";
    shockwave.spriteX = 8;
    shockwave.spriteY = 8;
    shockwave.framePerSecond = 128.f;
    shockwave.loop = false;
    shockwave.scaleOverTime = true;
    shockwave.startScale = 1.f;
    shockwave.endScale = 250.f;
    shockwave.startAlpha = 1.f;
    shockwave.endAlpha = 0.f;
    RegisterEffectTemplate(L"Shockwave", shockwave);
}


void CEffectManager::RegisterEffectTemplate(const std::wstring& name, const BillboardEffectDesc& desc)
{
    m_mapTemplates[name] = desc;
}

CAnimatedBillboardEffect* CEffectManager::SpawnEffect(const std::wstring& name, const Vec3& pos)
{
    auto it = m_mapTemplates.find(name);
    if (it == m_mapTemplates.end())
        return nullptr;

    auto& pool = m_mapPool[name];
    CAnimatedBillboardEffect* effect = nullptr;
    bool isNew = false;

    if (!pool.empty())
    {
        effect = pool.front();
        pool.pop();
    }
    else
    {
        effect = new CAnimatedBillboardEffect;
        effect->SetName(name);
        effect->Init(it->second);
        isNew = true;
    }

    effect->GetTransform()->SetRelativePosition(pos);
    effect->SetEnable(true);
    effect->Reset();

    if (isNew)
    {
        CLevelManager::GetInst()->GetCurrentLevel()->SafeAddGameObject(effect, 2, false);
    }

    m_activeEffects.push_back(effect);
    return effect;
}

CAnimatedBillboardEffect* CEffectManager::SpawnEffect(const std::wstring& name, const Vec3& pos, float startScale, float endScale)
{
    auto it = m_mapTemplates.find(name);
    if (it == m_mapTemplates.end())
        return nullptr;

    BillboardEffectDesc desc = it->second; 
    desc.startScale = startScale;
    desc.endScale = endScale;

    auto& pool = m_mapPool[name];
    CAnimatedBillboardEffect* effect = nullptr;
    bool isNew = false;

    if (!pool.empty())
    {
        effect = pool.front();
        pool.pop();
    }
    else
    {
        effect = new CAnimatedBillboardEffect;
        effect->SetName(name);
        effect->Init(desc);
        isNew = true;
    }

    effect->GetTransform()->SetRelativePosition(pos);
    effect->SetEnable(true);
    effect->SetDesc(desc); 
    effect->Reset();

    if (isNew)
    {
        CLevelManager::GetInst()->GetCurrentLevel()->SafeAddGameObject(effect, 2, false);
    }

    m_activeEffects.push_back(effect);
    return effect;
}

CAnimatedBillboardEffect* CEffectManager::SpawnEffect(const std::wstring& name, const Vec3& pos, const Vec3& direction, float speed, bool useGravity)
{
    auto it = m_mapTemplates.find(name);
    if (it == m_mapTemplates.end())
        return nullptr;

    auto desc = it->second; 
    desc.useRigidMove = true;
    desc.initialVelocity = direction * speed;
    desc.useGravity = useGravity;

    auto& pool = m_mapPool[name];
    CAnimatedBillboardEffect* effect = nullptr;
    bool isNew = false;

    if (!pool.empty())
    {
        effect = pool.front();
        pool.pop();
    }
    else
    {
        effect = new CAnimatedBillboardEffect;
        effect->SetName(name);
        effect->Init(desc);
        isNew = true;
    }

    effect->GetTransform()->SetRelativePosition(pos);
    effect->SetEnable(true);
    effect->SetDesc(desc); 
    effect->Reset();

    if (isNew)
    {
        CLevelManager::GetInst()->GetCurrentLevel()->SafeAddGameObject(effect, 2, false);
    }

    m_activeEffects.push_back(effect);
    return effect;
}

void CEffectManager::Update()
{
    for (auto it = m_activeEffects.begin(); it != m_activeEffects.end(); )
    {
        CAnimatedBillboardEffect* effect = *it;
        if (!effect->GetEnable())
        {
            m_mapPool[effect->GetName()].push(effect);
            it = m_activeEffects.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

void CEffectManager::SpawnRadialSmoke(const Vec3& centerPos, int smokeCount, float speed)
{
    for (int i = 0; i < smokeCount; ++i)
    {
        float angleDeg = 360.0f / smokeCount * i;
        float angleRad = XMConvertToRadians(angleDeg);

        Vec3 dir = Vec3(cosf(angleRad), 0.f, sinf(angleRad));
        dir.Normalize();
        SpawnEffect(L"Smoke", centerPos, dir, speed);
    }
}

void CEffectManager::Clear()
{
    for (auto& pair : m_mapPool)
    {
        auto& q = pair.second;
        while (!q.empty())
        {
            delete q.front();
            q.pop();
        }
    }
    m_mapPool.clear();

    for (auto* effect : m_activeEffects)
    {
        delete effect;
    }
    m_activeEffects.clear();

    m_mapTemplates.clear();
}
