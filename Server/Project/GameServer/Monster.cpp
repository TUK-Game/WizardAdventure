#include "pch.h"
#include "Monster.h"
#include "CreatureCollider.h"
#include "MonsterAI.h"

CMonster::CMonster()
{
	ObjectInfo = new Protocol::ObjectInfo();
	PosInfo = new Protocol::PosInfo();
	MonsterInfo = new Protocol::MonsterInfo();

	ObjectInfo->set_allocated_pos_info(PosInfo);
	MonsterInfo->set_allocated_object_info(ObjectInfo);

	m_BoxCollider = new CCreatureCollider();
	m_BoxCollider->SetOwner(this);

	m_Ai = new CMonsterAI();
	m_Ai->SetOwner(this);
}

CMonster::~CMonster()
{
	delete MonsterInfo;
}

void CMonster::Update(float deltaTime)
{
	CGameObject::Update(deltaTime);
	m_Ai->Update(deltaTime);
}

void CMonster::CollisionBegin(CBoxCollider* src, CBoxCollider* dest)
{
}

void CMonster::CollisionEvent(CBoxCollider* src, CBoxCollider* dest)
{
}
