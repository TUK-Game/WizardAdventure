#pragma once
#include "AnimatedBillboardEffect.h"

class CEffectManager : public CSingleton<CEffectManager>
{
    DECLARE_SINGLETON(CEffectManager)

public:
    void Init();
    void Clear();

public:
    void RegisterEffectTemplate(const std::wstring& name, const BillboardEffectDesc& desc);
    CAnimatedBillboardEffect* SpawnEffect(const std::wstring& name, const Vec3& pos);
    CAnimatedBillboardEffect* SpawnEffect(const std::wstring& name, const Vec3& pos, const Vec3& direction, float speed, bool useGravity = false);
    void SpawnRadialSmoke(const Vec3& centerPos, int smokeCount = 6, float speed = 240.f);

    void Update(); // 비활성화된 오브젝트 회수

private:

    std::unordered_map<std::wstring, BillboardEffectDesc> m_mapTemplates;
    std::unordered_map<std::wstring, std::queue<CAnimatedBillboardEffect*>> m_mapPool;
    std::vector<CAnimatedBillboardEffect*> m_activeEffects;
};

