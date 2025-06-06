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
#include "InventoryWIdgetWindow.h"
#include "StoreWidgetWindow.h"
#include "SkillData.h"
#include "AssetManager.h"

CSkillManager::CSkillManager(EPlayerAttribute attribute, CGameObject* owner)
    : m_Attribute(attribute), m_Owner(owner) {}

void CSkillManager::UseSkill(int skillIndex, float duration)
{
    CPlayer* player = CNetworkManager::GetInst()->s_GameSession->GetOwnPlayer();
    if (m_Owner != player)
        return;

    // skill index check
    if (skillIndex < 0 || skillIndex >= m_SkillSlots.size() || !m_SkillSlots[skillIndex])
        return;

    // cooldown check
    if (m_SkillCooldowns[skillIndex] > 0.f) {
        std::cout << "Skill is on cooldown: " << m_SkillCooldowns[skillIndex] << "s left\n";
        player->GetStateManager()->HandleEvent(player, "EndAttack");
        return;
    }

    // skill learn check
    std::shared_ptr<CSkillData> skill = m_SkillSlots[skillIndex];
    if (!skill)
        return;

    switch (skill->GetSkillType()) {
    case ESkillType::FireBallTowardQ:
        CastFireballTowardQ(skill->GetSkillInfo().damageRatio, duration);
        break;
    case ESkillType::FireTower:
        SpawnFireTowerAtMouse(skill->GetSkillInfo().damageRatio, duration);
        break;
    case ESkillType::FireSwordSpread:
        FireSwordSpreadShot(skill->GetSkillInfo().damageRatio, duration);
        break;
    case ESkillType::FireBallTowardMouse:
        CastFireballTowardMouse(skill->GetSkillInfo().damageRatio);
        break;
    case ESkillType::Meteor:
        CastMeteor(skill->GetSkillInfo().damageRatio);
        break;
    default:
        break;
    }
    m_SkillCooldowns[skillIndex] = skill->GetSkillInfo().cooltime;
 
}

void CSkillManager::LearnSkill(int slotIndex, std::shared_ptr<CSkillData> skill) {
    if (slotIndex < 0 || slotIndex >= m_SkillSlots.size())
        return;
    m_SkillSlots[slotIndex] = skill;

    {
        CInventoryWIdgetWindow* window = dynamic_cast<CInventoryWIdgetWindow*>(CLevelManager::GetInst()->GetCurrentLevel()->FindWidgetWindow(EWIDGETWINDOW_TYPE::INVENTORY_WINDOW));
        if (window && window->GetOwnerPlayer() == m_Owner)
        {
            window->SetSkill(slotIndex, skill->GetSkillType());
        }
    }

    {
        CStoreWidgetWindow* window = dynamic_cast<CStoreWidgetWindow*>(CLevelManager::GetInst()->GetCurrentLevel()->FindWidgetWindow(EWIDGETWINDOW_TYPE::STORE_WINDOW));
        if (window)
        {
            window->UpdateSkill(slotIndex, skill->GetSkillType());
        }
    }
}

void CSkillManager::ForgetSkill(int slotIndex) {
    if (slotIndex < 0 || slotIndex >= m_SkillSlots.size())
        return;
    m_SkillSlots[slotIndex] = nullptr;
}

ESkillType CSkillManager::GetEquippedSkill(int slotIndex) const {
    if (slotIndex < 0 || slotIndex >= m_SkillSlots.size() || !m_SkillSlots[slotIndex])
        return ESkillType::None;
    return m_SkillSlots[slotIndex]->GetSkillType();
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

void CSkillManager::CastFireballTowardMouse(float damageRatio)
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
    fireBall->SetDamage(damageRatio * fireBall->GetCaster()->GetStats()->attack);
    fireBall->SetEnable(false);
    fireBall->SetCollisionExplosion(true);

    CNetworkManager::GetInst()->s_GameSession->SpawnSkill(fireBall);

    CLevelManager::GetInst()->GetCurrentLevel()->SafeAddGameObject(fireBall, LAYER_PROJECTILE, false);
    CAssetManager::GetInst()->SoundPlay("FireBall");
}

void CSkillManager::CastFireballTowardQ(float damageRatio, float duration)
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
    fireBall->SetDamage(damageRatio * fireBall->GetCaster()->GetStats()->attack);
    fireBall->SetEnable(false);
    fireBall->SetCollisionExplosion(true);

    CRigidBody* rigidbody = fireBall->GetRigidBody();
    Vec3 velocity = fireDir * 400.f;
    rigidbody->SetVelocity(velocity);
    rigidbody->ApplyTorque(Vec3(0.f, 500.f, 0.f));
    rigidbody->SetAngularDrag(0.01f);

    CNetworkManager::GetInst()->s_GameSession->SpawnSkill(fireBall);

    CLevelManager::GetInst()->GetCurrentLevel()->SafeAddGameObject(fireBall, LAYER_PROJECTILE, false);
    CAssetManager::GetInst()->SoundPlay("FireBallQShoot");

}

void CSkillManager::SpawnFireTowerAtMouse(float damageRatio, float duration)
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
    tower->SetDamage(damageRatio * tower->GetCaster()->GetStats()->attack);
    tower->SetEnable(false);
    tower->SetCollisionExplosion(false);

    CNetworkManager::GetInst()->s_GameSession->SpawnSkill(tower);

    CLevelManager::GetInst()->GetCurrentLevel()->SafeAddGameObject(tower, LAYER_PROJECTILE, false);
    CAssetManager::GetInst()->SoundPlay("FireTower");

}

void CSkillManager::FireSwordSpreadShot(float damageRatio, float duration)
{
    if (!m_Owner) return;

    const int count = 8;
    const float radius = 80.f;
    Vec3 center = m_Owner->GetTransform()->GetRelativePosition();
    center.y += 100.f;
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
        sword->SetDamage(damageRatio * sword->GetCaster()->GetStats()->attack);
        sword->SetEnable(false);
        sword->SetCollisionExplosion(true);
        CNetworkManager::GetInst()->s_GameSession->SpawnSkill(sword);

        CLevelManager::GetInst()->GetCurrentLevel()->SafeAddGameObject(sword, LAYER_PROJECTILE, false);
    }

}

void CSkillManager::CastMeteor(float damageRatio)
{
    Vec3 centerPos = m_Owner->GetTransform()->GetRelativePosition();

    CMeteors* meteors = new CMeteors(centerPos, 25, 0.125f, damageRatio);
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