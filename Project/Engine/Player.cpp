﻿#include "pch.h"
#include "Player.h"
#include "StateManager.h"
#include "PlayerIdleState.h"
#include "PlayerRunState.h"
#include "PlayerDashState.h"
#include "PlayerAttackQState.h"
#include "PlayerAttackRState.h"
#include "PlayerAttackEState.h"
#include "PlayerAttackLButtonState.h"
#include "PlayerAttackRButtonState.h"
#include "PlayerKnockbackState.h"
#include "PlayerFallingState.h"
#include "PlayerDeathState.h"
#include "Transform.h"
#include "Engine.h"
#include "SkillManager.h"
#include <iostream>
#include "MeshData.h"
#include "AssetManager.h"
#include "BoxCollider.h"
#include "CollisionManager.h"
#include "PlayerScript.h"
#include "MeshRenderer.h"
#include "NetworkManager.h"
#include "ServerSession.h"
#include "SkillInfo.h"
#include "NPC.h"
#include "LevelManager.h"
#include "Level.h"
#include "RenderManager.h"
#include "Camera.h"
#include "CameraScript.h"
#include "SkillData.h"
#include "PlayerStateManager.h"
//#include <Engine/Engine.h>

CPlayer::CPlayer(EPlayerAttribute attribute, bool Owner, const Vec3& pos)
    : m_Attribute(attribute), m_SkillManager(new CSkillManager(attribute, this)), m_Interpolator(new CInterpolator())
{
    CreateStateManager();

    CMeshData* data = nullptr; 
    switch (attribute)
    {
    case EPlayerAttribute::Fire:
    {
        data = CAssetManager::GetInst()->FindAsset<CMeshData>(L"PurpleMage");
    }
    break;
    case EPlayerAttribute::Ice:
    {
        data = CAssetManager::GetInst()->FindAsset<CMeshData>(L"SkyMage");
    }
    break;
    case EPlayerAttribute::Electric:
    {
        data = CAssetManager::GetInst()->FindAsset<CMeshData>(L"YellowMage");
    }
    break;
    }
     
    std::vector<CGameObject*> obj = data->Instantiate(ECollision_Channel::Player);

   SetName(L"Mage");
   AddComponent(new CTransform);
   AddComponent(new CBoxCollider);
   GetCollider()->SetProfile(CCollisionManager::GetInst()->FindProfile("Player"));
   GetTransform()->SetRelativePosition(pos);
   m_Amount = Vec3(pos);
   m_NextPosition = Vec3(pos);
   GetCollider()->SetMaxMinPos(Vec3(pos), Vec3(100, 200, 24), Vec3(0, 0, 0), Vec3(0, 100, 0));
   if(Owner)
       AddComponent(new CPlayerScript);

#ifdef COLLISION_MESH_DRAW
    CCollisionObject* co = new CCollisionObject();
    co->InitToChild(player, Vec3(0, 100, 0), Vec3(100, 200, 24));
    player->AddChild(co);
#endif

    for (auto& o : obj)
    {
        std::wstring name = o->GetMeshRenderer()->GetMesh()->GetName();
        o->SetName(name);

        o->GetTransform()->SetRelativeScale(0.2f, 0.2f, 0.2f);
        Vec3 rot = o->GetTransform()->GetRelativeRotation();
        o->GetTransform()->SetRelativeRotation(rot);
        //o->GetTransform()->SetRelativePosition(2500.f, 577.f, -105.f);
        //o->AddComponent(new CTestPlayer);
        //o->GetMeshRenderer()->GetMaterial()->SetInt(0, 1);
        o->SetCheckFrustum(true);
        o->SetInstancing(false);
        this->AddChild(o);
    }

    m_Stats = new Stats;

    Begin();
}

CPlayer::~CPlayer()
{
    if (m_SkillManager)
        delete m_SkillManager;
    if (m_Interpolator)
        delete m_Interpolator;
    if (m_Stats)
        delete m_Stats;
}

void CPlayer::Begin()
{
    CGameObject::Begin();
    m_StateManager->ChangeState(this, EState_Type::Idle);
    m_Interpolator->SetTarget(GetTransform()->GetRelativePosition(), GetTransform()->GetRelativeRotation());
}

void CPlayer::Update()
{
    float time = DELTA_TIME;
    if (m_StateManager) {
        m_StateManager->Update(this, time);
    }
    m_SkillManager->UpdateCooldowns(time);

#ifndef DEBUG_SOLOPLAY
    m_Interpolator->Update(time);
    GetTransform()->SetRelativePosition((m_Interpolator->GetInterpolatedPos()));
    GetTransform()->SetRelativeRotation((m_Interpolator->GetInterpolatedRot()));
#endif // DEBUG_SOLOPLAY

    CGameObject::Update();


    constexpr float blink = 0.2f;
    if (m_bDamageDelay)
    {
        m_BlinkTime+= time;
        if (m_BlinkTime >= blink)
        {
            m_BlinkTime = 0.f;
            m_bRenderOn = !m_bRenderOn;
        }

        auto& childs = GetChild();
        for (auto child : childs)
        {
            child->SetIsRender(m_bRenderOn);
        }
    }
}

void CPlayer::FinalUpdate()
{
    CGameObject::FinalUpdate();
}

void CPlayer::Render()
{
    CGameObject::Render();
}

void CPlayer::AddSkill(std::shared_ptr<CSkillData> skill)
{
    const auto& skillManager = GetSkillManager();
    ESkillSlot slot = skillManager->GetNextSlot();
    assert(slot != ESkillSlot::END);
    skillManager->LearnSkill(slot, skill);
    skillManager->SetNextSlot();
}

void CPlayer::CreateStateManager()
{
    m_StateManager = new CPlayerStateManager();
    m_StateManager->AddState(new CPlayerIdleState);
    m_StateManager->AddState(new CPlayerRunState);
    m_StateManager->AddState(new CPlayerDashState);
    m_StateManager->AddState(new CPlayerAttackQState);
    m_StateManager->AddState(new CPlayerAttackRState);
    m_StateManager->AddState(new CPlayerAttackEState);
    m_StateManager->AddState(new CPlayerAttackEState);
    m_StateManager->AddState(new CPlayerAttackLButtonState);
    m_StateManager->AddState(new CPlayerAttackRButtonState);
    m_StateManager->AddState(new CPlayerKnockbackState);
    m_StateManager->AddState(new CPlayerDeathState);
    m_StateManager->AddState(new CPlayerFallingState);

    // idle -> others
    m_StateManager->SetTransition(EState_Type::Idle, "Move", EState_Type::Run);
    m_StateManager->SetTransition(EState_Type::Idle, "Dash", EState_Type::Dash);
    m_StateManager->SetTransition(EState_Type::Idle, "Attack_Q", EState_Type::Attack_Q);
    m_StateManager->SetTransition(EState_Type::Idle, "Attack_R", EState_Type::Attack_R);
    m_StateManager->SetTransition(EState_Type::Idle, "Attack_E", EState_Type::Attack_E);
    m_StateManager->SetTransition(EState_Type::Idle, "Attack_LButton", EState_Type::Attack_LButton);
    m_StateManager->SetTransition(EState_Type::Idle, "Attack_RButton", EState_Type::Attack_RButton);
    m_StateManager->SetTransition(EState_Type::Idle, "Knockback", EState_Type::Knockback);
    m_StateManager->SetTransition(EState_Type::Idle, "Death", EState_Type::Death);
    m_StateManager->SetTransition(EState_Type::Idle, "Fall", EState_Type::Falling);
    
    // run -> others
    m_StateManager->SetTransition(EState_Type::Run, "Stop", EState_Type::Idle);
    m_StateManager->SetTransition(EState_Type::Run, "Dash", EState_Type::Dash);
    m_StateManager->SetTransition(EState_Type::Run, "Attack_Q", EState_Type::Attack_Q);
    m_StateManager->SetTransition(EState_Type::Run, "Attack_R", EState_Type::Attack_R);
    m_StateManager->SetTransition(EState_Type::Run, "Attack_E", EState_Type::Attack_E);
    m_StateManager->SetTransition(EState_Type::Run, "Attack_LButton", EState_Type::Attack_LButton);
    m_StateManager->SetTransition(EState_Type::Run, "Attack_RButton", EState_Type::Attack_RButton);
    m_StateManager->SetTransition(EState_Type::Run, "Knockback", EState_Type::Knockback);
    m_StateManager->SetTransition(EState_Type::Run, "Death", EState_Type::Death);
    m_StateManager->SetTransition(EState_Type::Run, "Fall", EState_Type::Falling);

    // dash -> run
    m_StateManager->SetTransition(EState_Type::Dash, "EndDash", EState_Type::Run);
    m_StateManager->SetTransition(EState_Type::Dash, "Fall", EState_Type::Falling);

    // knockback -> idle
    m_StateManager->SetTransition(EState_Type::Knockback, "EndKnockback", EState_Type::Idle);
    m_StateManager->SetTransition(EState_Type::Knockback, "Fall", EState_Type::Falling);

    // attack -> idle
    m_StateManager->SetTransition(EState_Type::Attack_Q, "EndAttack", EState_Type::Idle);
    m_StateManager->SetTransition(EState_Type::Attack_R, "EndAttack", EState_Type::Idle);
    m_StateManager->SetTransition(EState_Type::Attack_E, "EndAttack", EState_Type::Idle);
    m_StateManager->SetTransition(EState_Type::Attack_LButton, "EndAttack", EState_Type::Idle);
    m_StateManager->SetTransition(EState_Type::Attack_RButton, "EndAttack", EState_Type::Idle);

    // attack -> death
    m_StateManager->SetTransition(EState_Type::Attack_Q, "Death", EState_Type::Death);
    m_StateManager->SetTransition(EState_Type::Attack_R, "Death", EState_Type::Death);
    m_StateManager->SetTransition(EState_Type::Attack_E, "Death", EState_Type::Death);
    m_StateManager->SetTransition(EState_Type::Attack_LButton, "Death", EState_Type::Death);
    m_StateManager->SetTransition(EState_Type::Attack_RButton, "Death", EState_Type::Death);

    // attack -> knockback
    m_StateManager->SetTransition(EState_Type::Attack_Q, "Knockback", EState_Type::Knockback);
    m_StateManager->SetTransition(EState_Type::Attack_R, "Knockback", EState_Type::Knockback);
    m_StateManager->SetTransition(EState_Type::Attack_E, "Knockback", EState_Type::Knockback);
    m_StateManager->SetTransition(EState_Type::Attack_LButton, "Knockback", EState_Type::Knockback);
    m_StateManager->SetTransition(EState_Type::Attack_RButton, "Knockback", EState_Type::Knockback);

    // temp
    m_StateManager->SetTransition(EState_Type::Death, "EndDeath", EState_Type::Idle);

    m_StateManager->SetTransition(EState_Type::Falling, "EndFall", EState_Type::Idle);
    m_StateManager->SetTransition(EState_Type::Falling, "Death", EState_Type::Death);
}

void CPlayer::Move(Vec3 moveDir, bool shouldRotate, float speedScale)
{
    CTransform* transform = GetTransform();
    if (!transform) return;

    if (moveDir.Length() > 0)
    {
        moveDir.Normalize(); 
        m_currentMoveDir = moveDir;
        m_Amount = moveDir * m_Speed * speedScale * CEngine::GetInst()->GetDeltaTime();
        m_NextPosition = transform->GetRelativePosition() + (moveDir * m_Speed * CEngine::GetInst()->GetDeltaTime());
#ifdef DEBUG_SOLOPLAY
        transform->SetRelativePosition(transform->GetRelativePosition() + m_Amount);
#endif

        if (shouldRotate) {
            float angle = atan2(moveDir.x, moveDir.z) * (180.0f / XM_PI); 
            transform->SetRelativeRotation(0.f, angle + 180.f, 0.f);
        }
#ifndef DEBUG_SOLOPLAY
        if(m_StateManager->GetCurrentStateType() != EState_Type::Dash)
        {
            CNetworkManager::GetInst()->s_GameSession->OnMovePlayer();
        }
#endif 
    }
}

void CPlayer::Attack(int skillIndex, float duration)
{
    m_SkillManager->UseSkill(skillIndex, duration);
#ifndef DEBUG_SOLOPLAY
    CNetworkManager::GetInst()->s_GameSession->OnActPlayer();
#endif 

}

void CPlayer::InitStats(int maxHp, int hp, int attack, float speed, int gold)
{
    m_Stats->maxHp = maxHp;
    m_Stats->currentHp = hp;
    m_Stats->attack = attack;
    m_Stats->moveSpeed = speed;
    m_Stats->gold = gold;
}

bool CPlayer::DetectNPC()
{
    CGameObject* camera = CRenderManager::GetInst()->GetMainCamera()->GetOwner();
    if (ECamera_Type::Fixed !=  camera->GetCamera()->GetCameraType())
        return false;

    CNPC* npc = CLevelManager::GetInst()->GetCurrentLevel()->DetectNPC(this);

    if (nullptr == npc)
        return false;
    npc->Interation();
    MoveCamera(camera, npc, ECamera_Type::Interaction_Start, Vec3(200.f, 0.f, 500.f));
    return true;
}

void CPlayer::MoveToInventoryView()
{
    CGameObject* camera = CRenderManager::GetInst()->GetMainCamera()->GetOwner();
    if (ECamera_Type::Fixed != camera->GetCamera()->GetCameraType())
        return;

    MoveCamera(camera, this, ECamera_Type::Inventory_Interaction_Start, Vec3(200.f, 0.f, 500.f));
}

void CPlayer::MoveCamera(CGameObject* camera, CGameObject* target, ECamera_Type type, const Vec3& offset)
{
    const auto& script = dynamic_cast<CCameraScript*>(camera->GetScript());
        
    if (script)
    {
        Vec3 rot;
        CTransform* transform = camera->GetTransform();
        script->SetInteractionZoomTarget(target->InteractionCameraPos(rot, offset));
        script->SetInteractionStartPos(transform->GetRelativePosition());
        script->SetInteractionDir(transform->GetRelativeRotation(), rot);
        camera->GetCamera()->SetCameraType(type);
    }
}

void CPlayer::FinishInteraction(bool isLayerCheck)
{
    CGameObject* camera = CRenderManager::GetInst()->GetMainCamera()->GetOwner();
    if(ECamera_Type::Interaction == camera->GetCamera()->GetCameraType())
    {
        camera->GetCamera()->SetCameraType(ECamera_Type::Interaction_End);
        if(isLayerCheck)
        {
            camera->GetCamera()->CheckLayer(LAYER_PLAYER);
        }
    }
}

void CPlayer::CollisionBegin(CBaseCollider* src, CBaseCollider* dest)
{
}


