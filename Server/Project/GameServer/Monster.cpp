#include "pch.h"
#include "Monster.h"
#include "CreatureCollider.h"
#include "MonsterAI.h"
#include "MonsterCollider.h"
#include "Room.h"
#include "Projectile.h"

CMonster::CMonster()
{
	if (ObjectInfo)
	{
		delete ObjectInfo;
	}

	ObjectInfo = new Protocol::ObjectInfo();
	PosInfo = new Protocol::PosInfo();
	MonsterInfo = new Protocol::MonsterInfo();

	ObjectInfo->set_allocated_pos_info(PosInfo);
	MonsterInfo->set_allocated_object_info(ObjectInfo);

	m_BoxCollider = new CMonsterCollider();
	m_BoxCollider->SetOwner(this);

	m_Ai = new CMonsterAI();
	m_Ai->SetOwner(this);
}

CMonster::~CMonster()
{
	delete MonsterInfo;
	delete m_Ai;
}

void CMonster::Update(float deltaTime)
{
	if (m_bActive)
	{
		CGameObject::Update(deltaTime);
		m_Ai->Update(deltaTime);
	}
}

void CMonster::CollisionBegin(CBoxCollider* src, CBoxCollider* dest)
{
	if (dest->GetProfile()->channel == ECollision_Channel::Projectile &&
		Protocol::MOVE_STATE_NONE != m_State)
	{
		if (GetAblity()->currentHp > 0)
		{
			Damaged((dynamic_cast<CProjectile*>(dest->GetOwner()))->GetAttack());
			if (GetAblity()->currentHp <= 0)
			{
				std::cout << "Á×À½\n";
				m_State = Protocol::MOVE_STATE_NONE;
				g_Room->RemoveObject((uint32)EObject_Type::Monster, MonsterInfo->object_id());
			}
		}
	}
}

void CMonster::CollisionEvent(CBoxCollider* src, CBoxCollider* dest)
{
}
