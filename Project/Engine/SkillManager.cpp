#include "pch.h"
#include "SkillManager.h"
#include "Transform.h"
#include "GameObject.h"
#include "InputManager.h"
#include "RenderManager.h"
#include "FireBall.h"
#include "FireSword.h"
#include "Meteors.h"
#include "FireTower.h"
#include "FireCircle.h"
#include "LevelManager.h"
#include "Level.h"
#include "RigidBody.h"
#include "Player.h"
#include "Camera.h"
#include "SkillDamage.h"
#include "ParticleSystemManager.h"
#include "ParticleSystem.h"
#include "ServerSession.h"
#include "NetworkManager.h"

CSkillManager::CSkillManager(EPlayerAttribute attribute, CGameObject* owner)
    : m_Attribute(attribute), m_Owner(owner) {}

void CSkillManager::UseSkill(int skillIndex, float duration)
{
    CPlayer* player = CNetworkManager::GetInst()->s_GameSession->GetOwnPlayer();
    if (m_Owner != player)
        return;

    switch (m_Attribute)
    {
    case EPlayerAttribute::Fire:
        if (skillIndex == 0)
            CastFireballTowardQ(duration);
        if (skillIndex == 1)
            SpawnFireTowerAtMouse(duration);
        if (skillIndex == 2)
            FireSwordSpreadShot();
        if (skillIndex == 3) 
            CastFireballTowardMouse();
        if (skillIndex == 4)
            CastMeteor();
        break;

    case EPlayerAttribute::Water:
        if (skillIndex == 0) std::cout << "Water Jet (Q)!" << std::endl;
        if (skillIndex == 1) std::cout << "Ice Shield (E)!" << std::endl;
        if (skillIndex == 2) std::cout << "Tsunami (R)!" << std::endl;
        if (skillIndex == 3) std::cout << "(LButton)!" << std::endl;
        if (skillIndex == 4) std::cout << "(RButton)!" << std::endl;
        break;

    case EPlayerAttribute::Electric:
        if (skillIndex == 0) std::cout << "Thunderbolt (Q)!" << std::endl;
        if (skillIndex == 1) std::cout << "Lightning Shield (E)!" << std::endl;
        if (skillIndex == 2) std::cout << "Storm Burst (R)!" << std::endl;
        if (skillIndex == 3) std::cout << "(LButton)!" << std::endl;
        if (skillIndex == 4) std::cout << "(RButton)!" << std::endl;
        break;
    }
}

void CSkillManager::CastFireballTowardMouse()
{
    CGameObject* player = m_Owner; 
    if (!player) return;

    Vec3 spawnPos = player->GetTransform()->GetWorldPosition();
    Vec3 fireDir = CalculateMouseDirectionFromPlayerTopView(spawnPos);

    SetLookRotationY(fireDir);

    CFireBall* fireBall = new CFireBall();
    fireBall->GetTransform()->SetRelativePosition(spawnPos);
    fireBall->SetDirection(fireDir);
    fireDir.Normalize();
    Vec3 velocity = fireDir * 3000.f;
    fireBall->GetRigidBody()->SetVelocity(velocity);

    fireBall->SetCaster(dynamic_cast<CPlayer*>(player));
    fireBall->SetDamage(SkillDamage::FireBall * fireBall->GetCaster()->GetStats()->attack);
    fireBall->SetEnable(false);
    fireBall->SetCollisionExplosion(true);

    CNetworkManager::GetInst()->s_GameSession->SpawnSkill(fireBall);

    CLevelManager::GetInst()->GetCurrentLevel()->SafeAddGameObject(fireBall, 12, false);
}

void CSkillManager::CastFireballTowardQ(float duration)
{
    CGameObject* player = m_Owner;
    if (!player) return;

    Vec3 spawnPos = player->GetTransform()->GetWorldPosition();
    Vec3 fireDir = CalculateMouseDirectionFromPlayerTopView(spawnPos);

    SetLookRotationY(fireDir);

    CFireBall* fireBall = new CFireBall();
    fireDir.Normalize();
    Vec3 offset = fireDir * 100.f;
    fireBall->GetTransform()->SetRelativePosition(spawnPos + offset);
    Vec3 startScale = Vec3(120.f, 120.f, 120.f);
    Vec3 endScale = Vec3(30.f, 30.f, 30.f);
    fireBall->GetTransform()->SetRelativeScale(startScale);
    fireBall->SetStartScale(startScale);
    fireBall->SetEndScale(endScale);

    fireBall->SetDuration(duration);
    fireBall->SetMode(EFireBallMode::QSkill);
    fireBall->SetCaster(dynamic_cast<CPlayer*>(player));
    fireBall->SetDamage(SkillDamage::FireBallQ * fireBall->GetCaster()->GetStats()->attack);
    fireBall->SetEnable(false);
    fireBall->SetCollisionExplosion(true);

    CRigidBody* rigidbody = fireBall->GetRigidBody();
    Vec3 velocity = fireDir * 400.f;
    rigidbody->SetVelocity(velocity);
    rigidbody->ApplyTorque(Vec3(0.f, 500.f, 0.f));
    rigidbody->SetAngularDrag(0.01f);

    CNetworkManager::GetInst()->s_GameSession->SpawnSkill(fireBall);

    CLevelManager::GetInst()->GetCurrentLevel()->SafeAddGameObject(fireBall, 12, false);
}

void CSkillManager::SpawnFireTowerAtMouse(float duration)
{
    Vec3 centerPos = GetMouseGroundPoint();

    //CFireCircle* fireCircle = new CFireCircle;
    //fireCircle->GetTransform()->SetRelativePosition(centerPos);
    //fireCircle->SetCaster(dynamic_cast<CPlayer*>(m_Owner));
    //CNetworkManager::GetInst()->s_GameSession->SpawnSkill(fireCircle);
    //CLevelManager::GetInst()->GetCurrentLevel()->SafeAddGameObject(fireCircle, 12, false);

    Vec3 lookDir = centerPos - m_Owner->GetTransform()->GetRelativePosition();
    lookDir.Normalize();
    SetLookRotationY(lookDir);

    CFireTower * tower = new CFireTower();
    tower->GetTransform()->SetRelativePosition(centerPos);
    tower->SetScaleDuration(duration);
    tower->SetCaster(dynamic_cast<CPlayer*>(m_Owner));
    tower->SetDamage(SkillDamage::Pillar * tower->GetCaster()->GetStats()->attack);
    tower->SetEnable(false);

    CNetworkManager::GetInst()->s_GameSession->SpawnSkill(tower);

    CLevelManager::GetInst()->GetCurrentLevel()->SafeAddGameObject(tower, 12, false);

}

void CSkillManager::FireSwordSpreadShot()
{
    if (!m_Owner) return;

    const int count = 8;
    const float radius = 80.f;
    Vec3 center = m_Owner->GetTransform()->GetRelativePosition();
    Vec3 targetPos = GetMouseGroundPoint();
    Vec3 lookDir = targetPos - center;
    lookDir.Normalize();
    SetLookRotationY(lookDir);
    for (int i = 0; i < count; ++i)
    {
        float angleDeg = i * (360.f / count) + 10.f;
        float angleRad = XMConvertToRadians(angleDeg);

        float offsetX = cosf(angleRad) * radius;
        float offsetZ = sinf(angleRad) * radius;
        Vec3 spawnPos = center + Vec3(offsetX, 300.f, offsetZ);
        Vec3 readyDir = spawnPos - center;
        readyDir.Normalize();

        CFireSword* sword = new CFireSword();
        sword->GetTransform()->SetRelativePosition(center);
        sword->GetTransform()->LookAt(readyDir);

        sword->SetReadyDirection(readyDir);
        sword->SetTargetPos(targetPos);
        sword->SetSpeed(800.f);              
        sword->SetWaitTimeForTranslate(1.5f);     
        sword->SetWaitTimeForRotate(1.f);
        sword->SetCaster(dynamic_cast<CPlayer*>(m_Owner));
        sword->SetDamage(SkillDamage::FireSword * sword->GetCaster()->GetStats()->attack);
        sword->SetEnable(false);
        sword->SetCollisionExplosion(true);
        CNetworkManager::GetInst()->s_GameSession->SpawnSkill(sword);

        CLevelManager::GetInst()->GetCurrentLevel()->SafeAddGameObject(sword, 12, false);
    }

}

void CSkillManager::CastMeteor()
{
    Vec3 centerPos = m_Owner->GetTransform()->GetRelativePosition();

    CMeteors* meteors = new CMeteors(centerPos, 25, 0.125f);
    meteors->SetCaster(dynamic_cast<CPlayer*>(m_Owner));


    CLevelManager::GetInst()->GetCurrentLevel()->SafeAddGameObject(meteors, 12, false);
}

Vec3 CSkillManager::CalculateMouseDirectionFromPlayerTopView(const Vec3& fromPos)
{
    Vec2 mousePos = CInputManager::GetInst()->GetMousePosition();
    CCamera* cam = CRenderManager::GetInst()->GetMainCamera();
    if (!cam) return Vec3(0, 0, 1);

    Vec3 rayOrigin, rayDir;
    cam->ScreenToRay(mousePos, cam, rayOrigin, rayDir);

    float t = (fromPos.y - rayOrigin.y) / rayDir.y;
    Vec3 hitPos = rayOrigin + rayDir * t;

    Vec3 dir = hitPos - fromPos;
    dir.y = 0.f; 
    if (dir.Length() > 0.001f)
        dir.Normalize();
    else
        dir = Vec3(0, 0, 1);

    return dir;
}

Vec3 CSkillManager::CalculateMouseDirectionFromPos(const Vec3& fromPos)
{
    Vec2 mousePos = CInputManager::GetInst()->GetMousePosition();
    CCamera* cam = CRenderManager::GetInst()->GetMainCamera();
    if (!cam) return Vec3(0, 0, 1);

    Vec3 rayOrigin, rayDir;
    cam->ScreenToRay(mousePos, cam, rayOrigin, rayDir);

    float t = -rayOrigin.y / rayDir.y;
    Vec3 hitPos = rayOrigin + rayDir * t;

    Vec3 dir = hitPos - fromPos;
    if (dir.Length() > 0.001f)
        dir.Normalize();
    else
        dir = Vec3(0, 0, 1);

    return dir;
}



Vec3 CSkillManager::GetMouseGroundPoint()
{
    Vec2 mousePos = CInputManager::GetInst()->GetMousePosition();
    CCamera* cam = CRenderManager::GetInst()->GetMainCamera();
    if (!cam) return Vec3::Zero;

    Vec3 origin, dir;
    cam->ScreenToRay(mousePos, cam, origin, dir);

    float t = -origin.y / dir.y;
    Vec3 hitPos = origin + dir * t;
    return hitPos;
}

void CSkillManager::SetLookRotationY(const Vec3& dir)
{

    float angle = atan2(dir.x, dir.z) * (180.0f / XM_PI);
    m_Owner->GetTransform()->SetRelativeRotation(0.f, angle + 180.f, 0.f);
}