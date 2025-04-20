#include "pch.h"
#include "Meteors.h"
#include "FireBall.h"
#include "Transform.h"
#include "MeshRenderer.h"
#include "RigidBody.h"
#include "AssetManager.h"
#include "Level.h"
#include "Layer.h"
#include "LevelManager.h"
#include "Engine.h"
#include "SkillDamage.h"
#include "ServerSession.h"
#include "NetworkManager.h"

CMeteors::CMeteors(Vec3 centerPos, int count, float interval)
    : m_CenterPos(centerPos), m_TotalCount(count), m_Interval(interval)
{
    m_type = SKILL::FIRE_METEORS;
    AddComponent(new CTransform());
}

void CMeteors::Update()
{
    CSkillObject::Update();
    if (m_bOwn)
    {
        m_ElapsedTime += DELTA_TIME;
        if (m_ElapsedTime >= m_Interval)
        {
            m_ElapsedTime = 0.f;
            SpawnMeteor();
            ++m_SpawnedCount;
        }
    }
}

void CMeteors::FinalUpdate()
{
    CGameObject::FinalUpdate();
    if (m_bOwn)
    {
        if (m_SpawnedCount >= m_TotalCount) {
            CLevelManager::GetInst()->GetCurrentLevel()->GetLayer(GetLayerIndex())->SafeRemoveGameObject(this);
        }
    }
}


void CMeteors::SpawnMeteor()
{
    float radius = 500.f;
    float theta = RandomFloat(0.f, XM_2PI);
    float r = sqrtf(RandomFloat(0.f, 1.f)) * radius;

    float offsetX = cosf(theta) * r;
    float offsetZ = sinf(theta) * r;
    Vec3 spawnPos = m_CenterPos + Vec3(offsetX, RandomFloat(1300.f, 1500.f), RandomFloat(1000.f, 2000.f));

    CFireBall* meteor = new CFireBall();
    meteor->GetTransform()->SetRelativePosition(spawnPos);

    float scale = RandomFloat(50.f, 150.f);
    meteor->GetTransform()->SetRelativeScale(scale, scale, scale);
    meteor->SetDuration(5.5f);
    meteor->SetCaster(GetCaster());
    meteor->SetDamage(SkillDamage::Meteor);
    meteor->UseSmokeTrail();
    meteor->SetEnable(false);

    CRigidBody* rigidbody = meteor->GetRigidBody();
    rigidbody->SetGravity(true);
    rigidbody->SetDrag(0.f);
    rigidbody->SetVelocity(Vec3(RandomFloat(-300.f, 300.f), -1500.f, RandomFloat(-2000.f, -1000.f)));

    CNetworkManager::GetInst()->s_GameSession->SpawnSkill(meteor);

    CLevelManager::GetInst()->GetCurrentLevel()->SafeAddGameObject(meteor, 12, false);
}