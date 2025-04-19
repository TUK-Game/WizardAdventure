#include "pch.h"
#include "SkillObject.h"
#include "Player.h"
#include "Level.h"
#include "Layer.h"
#include "LevelManager.h"
#include "BaseCollider.h"
#include "Engine.h"
#include "NetworkManager.h"
#include "ServerSession.h"


void CSkillObject::SetCaster(CPlayer* caster)
{
    m_Caster = caster;
    if (caster == CNetworkManager::GetInst()->s_GameSession->GetOwnPlayer())
    {
        m_bOwn = true;
    }
}

void CSkillObject::Update()
{
    CGameObject::Update();
    if(m_bOwn)
    {
        CNetworkManager::GetInst()->s_GameSession->MoveSkill(this);
    }
}

void CSkillObject::FinalUpdate()
{
    CGameObject::FinalUpdate();
}

void CSkillObject::OnHit(CGameObject* target)
{
    if (m_Caster)
    {
        int casterAtk = m_Caster->GetStats().attack;
        int totalDamage = m_Damage + casterAtk; 

        target->ReceiveDamage(totalDamage);

    }
}

void CSkillObject::CollisionBegin(CBaseCollider* src, CBaseCollider* dest)
{
    if (dest == nullptr || m_Caster == nullptr)
        return;

    CGameObject* target = dest->GetOwner();
    if (target)
    {
        OnHit(target);
        // CLevelManager::GetInst()->GetCurrentLevel()->GetLayer(GetLayerIndex())->SafeRemoveGameObject(this);
    }
}