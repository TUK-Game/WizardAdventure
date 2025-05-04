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
#include "SkillInfo.h"
#include "ParticleSystemManager.h"
#include "ParticleSystem.h"
#include "ServerSession.h"
#include "NetworkManager.h"
#include "StateManager.h"

CSkillManager::CSkillManager(EPlayerAttribute attribute, CGameObject* owner)
    : m_Attribute(attribute), m_Owner(owner) {}

void CSkillManager::UseSkill(int skillIndex, float duration)
{
    CPlayer* player = CNetworkManager::GetInst()->s_GameSession->GetOwnPlayer();
    if (m_Owner != player)
        return;

    // skill index check
    if (skillIndex < 0 || skillIndex >= m_SkillSlots.size())
        return;

    // cooldown check
    if (m_SkillCooldowns[skillIndex] > 0.f) {
        std::cout << "Skill is on cooldown: " << m_SkillCooldowns[skillIndex] << "s left\n";
        player->GetStateManager()->HandleEvent(player, "EndAttack");
        return;
    }

    // skill learn check
    ESkillType skill = m_SkillSlots[skillIndex];
    if (skill == ESkillType::None)
        return;


    switch (skill) {
    case ESkillType::FireBallTowardQ:
        CastFireballTowardQ(duration);
        m_SkillCooldowns[skillIndex] = Skill::FireBallQ.cooldown;
        break;
    case ESkillType::FireTower:
        SpawnFireTowerAtMouse(duration);
        m_SkillCooldowns[skillIndex] = Skill::FireTower.cooldown;
        break;
    case ESkillType::FireSwordSpread:
        FireSwordSpreadShot(duration);
        m_SkillCooldowns[skillIndex] = Skill::FireSword.cooldown;
        break;
    case ESkillType::FireBallTowardMouse:
        CastFireballTowardMouse();
        m_SkillCooldowns[skillIndex] = Skill::FireBall.cooldown;
        break;
    case ESkillType::Meteor:
        CastMeteor();
        m_SkillCooldowns[skillIndex] = Skill::Meteor.cooldown;
        break;
    default:
        break;
    }
 
}

void CSkillManager::LearnSkill(int slotIndex, ESkillType skill) {
    if (slotIndex < 0 || slotIndex >= m_SkillSlots.size())
        return;
    m_SkillSlots[slotIndex] = skill;
}

void CSkillManager::ForgetSkill(int slotIndex) {
    if (slotIndex < 0 || slotIndex >= m_SkillSlots.size())
        return;
    m_SkillSlots[slotIndex] = ESkillType::None;
}

ESkillType CSkillManager::GetEquippedSkill(int slotIndex) const {
    if (slotIndex < 0 || slotIndex >= m_SkillSlots.size())
        return ESkillType::None;
    return m_SkillSlots[slotIndex];
}

void CSkillManager::UpdateCooldowns(float deltaTime) {
    for (float& time : m_SkillCooldowns) {
        if (time > 0.f) {
            time -= deltaTime;
            if (time < 0.f) time = 0.f;
        }
    }
}

float CSkillManager::GetSkillCooldown(int slotIndex) const
{
    if (slotIndex < 0 || slotIndex >= m_SkillCooldowns.size())
        return 0.f;

    return m_SkillCooldowns[slotIndex];
}

void CSkillManager::CastFireballTowardMouse()
{
    CGameObject* player = m_Owner; 
    if (!player) return;

    Vec3 spawnPos = player->GetTransform()->GetWorldPosition();
    Vec3 fireDir = CalculateMouseDirectionFromPlayerTopView(spawnPos);

    SetLookRotationY(fireDir);

    CFireBall* fireBall = new CFireBall();
    Vec3 offset = fireDir * 50.f + Vec3(0.f, 130.f, 0.f);
    fireBall->GetTransform()->SetRelativePosition(spawnPos + offset);
    fireBall->SetDirection(fireDir);
    fireDir.Normalize();
    Vec3 velocity = fireDir * 3000.f;
    fireBall->GetRigidBody()->SetVelocity(velocity);

    fireBall->SetCaster(dynamic_cast<CPlayer*>(player));
    fireBall->SetDamage(Skill::FireBall.damage * fireBall->GetCaster()->GetStats()->attack);
    fireBall->SetEnable(false);
    fireBall->SetCollisionExplosion(true);

    CNetworkManager::GetInst()->s_GameSession->SpawnSkill(fireBall);

    CLevelManager::GetInst()->GetCurrentLevel()->SafeAddGameObject(fireBall, LAYER_PROJECTILE, false);
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
    Vec3 offset = fireDir * 100.f + Vec3(0.f, 130.f, 0.f);
    fireBall->GetTransform()->SetRelativePosition(spawnPos + offset);
    Vec3 startScale = Vec3(120.f, 120.f, 120.f);
    Vec3 endScale = Vec3(30.f, 30.f, 30.f);
    fireBall->GetTransform()->SetRelativeScale(startScale);
    fireBall->SetStartScale(startScale);
    fireBall->SetEndScale(endScale);

    fireBall->SetDuration(duration);
    fireBall->SetMode(EFireBallMode::QSkill);
    fireBall->SetSkillType(SKILL::FIRE_BALL_EXPLOSION);
    fireBall->SetCaster(dynamic_cast<CPlayer*>(player));
    fireBall->SetDamage(Skill::FireBallQ.damage * fireBall->GetCaster()->GetStats()->attack);
    fireBall->SetEnable(false);
    fireBall->SetCollisionExplosion(true);

    CRigidBody* rigidbody = fireBall->GetRigidBody();
    Vec3 velocity = fireDir * 400.f;
    rigidbody->SetVelocity(velocity);
    rigidbody->ApplyTorque(Vec3(0.f, 500.f, 0.f));
    rigidbody->SetAngularDrag(0.01f);

    CNetworkManager::GetInst()->s_GameSession->SpawnSkill(fireBall);

    CLevelManager::GetInst()->GetCurrentLevel()->SafeAddGameObject(fireBall, LAYER_PROJECTILE, false);
}

void CSkillManager::SpawnFireTowerAtMouse(float duration)
{
    Vec3 centerPos = GetMouseGroundPoint();

    Vec3 lookDir = centerPos - m_Owner->GetTransform()->GetRelativePosition();
    lookDir.Normalize();
    SetLookRotationY(lookDir);

    CFireTower * tower = new CFireTower();
    tower->GetTransform()->SetRelativePosition(centerPos);
    tower->SetScaleDuration(duration);
    tower->SetCaster(dynamic_cast<CPlayer*>(m_Owner));
    tower->Init(m_Owner); 
    tower->SetDamage(Skill::FireTower.damage * tower->GetCaster()->GetStats()->attack);
    tower->SetEnable(false);
    tower->SetCollisionExplosion(false);

    CNetworkManager::GetInst()->s_GameSession->SpawnSkill(tower);

    CLevelManager::GetInst()->GetCurrentLevel()->SafeAddGameObject(tower, LAYER_PROJECTILE, false);

}

void CSkillManager::FireSwordSpreadShot(float duration)
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
        sword->SetSpeed(3000.f);              
        sword->SetWaitTimeForTranslate(duration);
        float rotateDuration = duration * 4.f;
        float rotateDurationDivideByCount = rotateDuration / static_cast<float>(count);
        sword->SetWaitTimeForRotate(rotateDurationDivideByCount * i + 0.3f);
        sword->SetCaster(dynamic_cast<CPlayer*>(m_Owner));
        sword->SetDamage(Skill::FireSword.damage * sword->GetCaster()->GetStats()->attack);
        sword->SetEnable(false);
        sword->SetCollisionExplosion(true);
        CNetworkManager::GetInst()->s_GameSession->SpawnSkill(sword);

        CLevelManager::GetInst()->GetCurrentLevel()->SafeAddGameObject(sword, LAYER_PROJECTILE, false);
    }

}

void CSkillManager::CastMeteor()
{
    Vec3 centerPos = m_Owner->GetTransform()->GetRelativePosition();

    CMeteors* meteors = new CMeteors(centerPos, 25, 0.125f);
    meteors->SetCaster(dynamic_cast<CPlayer*>(m_Owner));


    CLevelManager::GetInst()->GetCurrentLevel()->SafeAddGameObject(meteors, LAYER_PROJECTILE, false);
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