#include "pch.h"
#include "Monster.h"
#include "CreatureCollider.h"

CMonster::CMonster()
{
	ObjectInfo = new Protocol::ObjectInfo();
	PosInfo = new Protocol::PosInfo();
	MonsterInfo = new Protocol::MonsterInfo();

	ObjectInfo->set_allocated_pos_info(PosInfo);
	MonsterInfo->set_allocated_object_info(ObjectInfo);

	m_BoxCollider = new CCreatureCollider();
	m_BoxCollider->SetOwner(this);
}

CMonster::~CMonster()
{
	delete MonsterInfo;
}

void CMonster::Update()
{
	CGameObject::Update();
}

void CMonster::CollisionBegin(CBoxCollider* src, CBoxCollider* dest)
{
}

void CMonster::CollisionEvent(CBoxCollider* src, CBoxCollider* dest)
{
}
