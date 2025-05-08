#include "pch.h"
#include "MonsterDissolveState.h"
#include "GameObject.h"
#include "MonsterAI.h"
#include "StateManager.h"
#include "AssetManager.h"
#include "Material.h"
#include "MeshRenderer.h"

void CMonsterDissolveState::Enter(CGameObject* entity)
{
#ifdef _DEBUG
	std::cout << "Entering Dissolve State" << std::endl;
#endif
    entity->SetThreshold(0.f);
    std::vector<CGameObject*> objs = entity->GetChild();
    for (auto o : objs) {
        if (o->GetAnimator()) {
            // 디졸브 셰이더 교체
            auto renderer = o->GetMeshRenderer();
            if (renderer) {
                auto dissolveMat = CAssetManager::GetInst()->FindAsset<CMaterial>(L"Dissolve");
                if (dissolveMat) {
                    dissolveMat->SetTexture(0, renderer->GetMaterial()->GetTexture(0));
                    dissolveMat->SetTexture(1, renderer->GetMaterial()->GetTexture(1));
                    dissolveMat->SetFloat(0, entity->GetThreshold()); // threshold
                    renderer->SetMaterial(dissolveMat);

                }
            }
        }
    }
    m_ElapsedTime = 0.f;

}

void CMonsterDissolveState::Update(CGameObject* entity, float deltaTime)
{
    m_ElapsedTime += deltaTime;

    float threshold = m_ElapsedTime / m_DissolveDuration;
    threshold = std::clamp(threshold, 0.f, 1.f);
    entity->SetThreshold(threshold);

    std::vector<CGameObject*> objs = entity->GetChild();
    for (auto o : objs) {
        auto renderer = o->GetMeshRenderer();
        if (renderer) {
            auto mat = renderer->GetMaterial();
            if (mat) {
                mat->SetFloat(0, threshold); // MATERIAL_PARAMS의 float_0으로 전달
            }
        }
    }

    if (threshold >= 1.0f) {
        m_ElapsedTime = 0.f;
    }

}

void CMonsterDissolveState::Exit(CGameObject* entity)
{
#ifdef _DEBUG
	std::cout << "Exiting Dissolve State" << std::endl;
#endif
}