#include "pch.h"
#include "FireTower.h"
#include "Transform.h"
#include "MeshRenderer.h"
#include "RigidBody.h"
#include "AssetManager.h"
#include "Level.h"
#include "Layer.h"
#include "LevelManager.h"
#include "Engine.h"
#include "MeshData.h"
#include "EffectManager.h"
#include "FireBall.h"
#include "NetworkManager.h"
#include "ServerSession.h"
#include "SkillDamage.h"

CFireTower::CFireTower()
{
    m_type = SKILL::FIRE_PILLAR;
    AddComponent(new CTransform());
    CMeshData* data2 = CAssetManager::GetInst()->FindAsset<CMeshData>(L"FireTower");
    std::vector<CGameObject*> obj2 = data2->Instantiate(ECollision_Channel::Player); // temp
    for (auto& o : obj2)
    {
        std::wstring name = o->GetMeshRenderer()->GetMesh()->GetName();
        o->SetName(name);
        Vec3 rot = o->GetTransform()->GetRelativeRotation();
        o->GetTransform()->SetRelativeRotation(rot);
        o->SetCheckFrustum(true);
        o->SetInstancing(false);
        AddChild(o);
    }
    // AddComponent(new CCollider());      
    
}

void CFireTower::Update()
{
    if (m_bOwn)
    {
        if (!m_bFinishScale) {
            UpdateScaleLerp();
        }
        else
        {
            // 공격 가능 상태가 되었을 때만 공격 로직 수행
            m_TimeSinceLastAttack += DELTA_TIME;
            TryAttack();
        }
    }
    CSkillObject::Update();
}

void CFireTower::FinalUpdate()
{
    CGameObject::FinalUpdate();
    if (m_bOwn)
    {
        m_ElapsedTime += DELTA_TIME;
        if (m_ElapsedTime >= m_Duration) {
            if (m_FireEffect) {
                m_FireEffect->SetEnable(false);
                m_FireEffect = nullptr; 
            }
            m_bDelete = true;
        }
    }
}

void CFireTower::UpdateScaleLerp()
{
    if (m_ElapsedTime >= m_ScaleDuration) {
        m_bFinishScale = true;
        m_bCanAttack = true; // attack on

        auto pos = GetTransform()->GetRelativePosition();
        pos += Vec3(0.f, 250.f, 0.f);
        m_FireEffect = CEffectManager::GetInst()->SpawnEffect(L"fire", pos);
        return;
    }
    float t = m_ElapsedTime / m_ScaleDuration;
    t = std::clamp(t, 0.f, 1.f);
    GetTransform()->SetRelativeScale(1.f, t, 1.f);
}

void CFireTower::TryAttack()
{
    if (!m_bCanAttack || m_TimeSinceLastAttack < m_AttackCooldown)
        return;

    Vec3 myPos = GetTransform()->GetWorldPosition();

    auto enemies = CLevelManager::GetInst()->GetCurrentLevel()->GetLayer(11)->GetObjects();
    for (auto enemy : enemies)
    {
        Vec3 enemyPos = enemy->GetTransform()->GetRelativePosition();
        float distance = (enemyPos - myPos).Length();

        if (distance <= m_AttackRange)
        {
            FireAtEnemy(enemy);
            m_TimeSinceLastAttack = 0.f;
            break; 
        }
    }
}

void CFireTower::FireAtEnemy(CGameObject* enemy)
{

    Vec3 pos = GetTransform()->GetRelativePosition();
    pos += Vec3(0.f, 250.f, 0.f);

    if (!enemy)
        return;

    Vec3 targetPos = enemy->GetTransform()->GetRelativePosition();
    Vec3 fireDir = targetPos - pos;

    CFireBall* fireBall = new CFireBall();
    fireBall->GetTransform()->SetRelativePosition(pos);
    fireDir.Normalize();
    Vec3 velocity = fireDir * 3000.f;
    fireBall->GetRigidBody()->SetVelocity(velocity);

    fireBall->SetDamage(SkillDamage::FireTower/* * fireBall->GetCaster()->GetStats()->attack*/);
    fireBall->SetEnable(false);
    fireBall->SetCollisionExplosion(true);
    fireBall->SetMode(EFireBallMode::Default);

    CNetworkManager::GetInst()->s_GameSession->SpawnSkill(fireBall);

    CLevelManager::GetInst()->GetCurrentLevel()->SafeAddGameObject(fireBall, 12, false);
}