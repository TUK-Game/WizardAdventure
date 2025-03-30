#include "pch.h"
#include "SkillManager.h"
#include "Transform.h"
#include "GameObject.h"
#include "InputManager.h"
#include "RenderManager.h"
#include "FireBall.h"
#include "Meteors.h"
#include "FirePillar.h"
#include "FireCircle.h"
#include "LevelManager.h"
#include "Level.h"
#include "RigidBody.h"
#include "Camera.h"

CSkillManager::CSkillManager(EPlayerAttribute attribute, CGameObject* owner)
    : m_Attribute(attribute), m_Owner(owner) {}

void CSkillManager::UseSkill(int skillIndex)
{
    switch (m_Attribute)
    {
    case EPlayerAttribute::Fire:
        if (skillIndex == 0)
            CastFireballTowardQ();
        if (skillIndex == 1)
            SpawnFirePillarAtMouse();
        if (skillIndex == 2)
            CastMeteor();
        if (skillIndex == 3) 
            CastFireballTowardMouse();
        break;

    case EPlayerAttribute::Water:
        if (skillIndex == 0) std::cout << "Water Jet (Q)!" << std::endl;
        if (skillIndex == 1) std::cout << "Ice Shield (E)!" << std::endl;
        if (skillIndex == 2) std::cout << "Tsunami (R)!" << std::endl;
        if (skillIndex == 3) std::cout << "(LButton)!" << std::endl;
        break;

    case EPlayerAttribute::Electric:
        if (skillIndex == 0) std::cout << "Thunderbolt (Q)!" << std::endl;
        if (skillIndex == 1) std::cout << "Lightning Shield (E)!" << std::endl;
        if (skillIndex == 2) std::cout << "Storm Burst (R)!" << std::endl;
        if (skillIndex == 3) std::cout << "(LButton)!" << std::endl;
        break;
    }
}


void CSkillManager::CastFireballTowardMouse()
{
    CGameObject* player = m_Owner; 
    if (!player) return;

    Vec3 spawnPos = player->GetTransform()->GetWorldPosition();
    Vec3 fireDir = CalculateMouseDirectionFromPlayerTopView(spawnPos);

    float angle = atan2(fireDir.x, fireDir.z) * (180.0f / XM_PI);
    player->GetTransform()->SetRelativeRotation(0.f, angle + 180.f, 0.f);

    CFireBall* fireBall = new CFireBall();
    fireBall->GetTransform()->SetRelativePosition(spawnPos);
    fireBall->SetDirection(fireDir);
    fireBall->SetSpeed(1200.f);
    fireBall->GetRigidBody()->ApplyForce(fireDir * 900000.f);

    CLevelManager::GetInst()->GetCurrentLevel()->SafeAddGameObject(fireBall, 3, false);
}

void CSkillManager::CastFireballTowardQ()
{
    CGameObject* player = m_Owner;
    if (!player) return;

    Vec3 spawnPos = player->GetTransform()->GetWorldPosition();
    Vec3 fireDir = CalculateMouseDirectionFromPlayerTopView(spawnPos);

    float angle = atan2(fireDir.x, fireDir.z) * (180.0f / XM_PI);
    player->GetTransform()->SetRelativeRotation(0.f, angle + 180.f, 0.f);

    CFireBall* fireBall = new CFireBall();
    fireBall->GetTransform()->SetRelativePosition(spawnPos);
    fireBall->GetTransform()->SetRelativeScale(120.f, 120.f, 120.f);
    fireBall->SetDirection(fireDir);
    fireBall->SetDuration(2.5f);
    fireBall->SetSpeed(1200.f);
    CRigidBody* rigidbody = fireBall->GetRigidBody();
    rigidbody->ApplyForce(fireDir * 70000.f);
    rigidbody->ApplyTorque(Vec3(0.f, 500.f, 0.f));
    rigidbody->SetAngularDrag(0.01f);

    CLevelManager::GetInst()->GetCurrentLevel()->SafeAddGameObject(fireBall, 3, false);
}


void CSkillManager::SpawnFirePillarAtMouse()
{
    Vec3 centerPos = GetMouseGroundPoint(); // 마우스 위치 (XZ 평면)

    CFireCircle* fireCircle = new CFireCircle;
    fireCircle->GetTransform()->SetRelativePosition(centerPos);
    CLevelManager::GetInst()->GetCurrentLevel()->SafeAddGameObject(fireCircle, 3, false);


    centerPos.y = RandomFloat(-200.f, -150.f);
    int count = 5;
    float radius = 150.f;
    for (int i = 0; i < count; ++i) {
        float angleDeg = i * (360.f / count);
        float angleRad = XMConvertToRadians(angleDeg);

        float offsetX = cosf(angleRad) * radius;
        float offsetZ = sinf(angleRad) * radius;

        Vec3 spawnPos = centerPos + Vec3(offsetX, 0.f, offsetZ); // y는 지면 아래로

        CFirePillar* pillar = new CFirePillar();
        pillar->SetBasePos(spawnPos);
        pillar->GetTransform()->SetRelativePosition(spawnPos);

        CLevelManager::GetInst()->GetCurrentLevel()->SafeAddGameObject(pillar, 3, false);
    }                                                                                               
    
}

void CSkillManager::CastMeteor()
{
    Vec3 centerPos = m_Owner->GetTransform()->GetRelativePosition();

    CMeteors* meteors = new CMeteors(centerPos, 20, 0.125f);
    CLevelManager::GetInst()->GetCurrentLevel()->SafeAddGameObject(meteors, 3, false);
}

Vec3 CSkillManager::CalculateMouseDirectionFromPlayerTopView(const Vec3& fromPos)
{
    Vec2 mousePos = CInputManager::GetInst()->GetMousePosition();
    CCamera* cam = CRenderManager::GetInst()->GetMainCamera();
    if (!cam) return Vec3(0, 0, 1);

    Vec3 rayOrigin, rayDir;
    cam->ScreenToRay(mousePos, cam, rayOrigin, rayDir);

    // XZ 평면(Y = fromPos.y) 기준 교차점
    float t = (fromPos.y - rayOrigin.y) / rayDir.y;
    Vec3 hitPos = rayOrigin + rayDir * t;

    Vec3 dir = hitPos - fromPos;
    dir.y = 0.f; // y 방향 제거 (수평 발사)
    if (dir.Length() > 0.001f)
        dir.Normalize();
    else
        dir = Vec3(0, 0, 1); // 기본 방향

    return dir;
}


Vec3 CSkillManager::GetMouseGroundPoint()
{
    Vec2 mousePos = CInputManager::GetInst()->GetMousePosition();
    CCamera* cam = CRenderManager::GetInst()->GetMainCamera();
    if (!cam) return Vec3::Zero;

    Vec3 origin, dir;
    cam->ScreenToRay(mousePos, cam, origin, dir);

    // 평면 Y = 0 (지면과 교차)
    float t = -origin.y / dir.y;
    Vec3 hitPos = origin + dir * t;
    return hitPos;
}
