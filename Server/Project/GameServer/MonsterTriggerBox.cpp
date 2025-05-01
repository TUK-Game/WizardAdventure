#include "pch.h"
#include "MonsterTriggerBox.h"
#include "BoxCollider.h"
#include "Room.h"
#include "LevelCollision.h"
#include "Monster.h"
#include "MonsterArea.h"

CMonseterTriggerBox::CMonseterTriggerBox()
{
}

CMonseterTriggerBox::~CMonseterTriggerBox()
{
}

void CMonseterTriggerBox::Init()
{

}


bool CMonseterTriggerBox::IsMonsterInArea(const Vec3& pos)
{
	Vec3 min = m_AreaCenter - (m_AreaSize * 0.5f);
	Vec3 max = m_AreaCenter + (m_AreaSize * 0.5f);

	return (pos.x >= min.x && pos.x <= max.x && pos.z >= min.z && pos.z <= max.z);
}

void CMonseterTriggerBox::CollisionBegin(CBoxCollider* src, CBoxCollider* dest)
{
	//std::cout << "트리거 박스 발동\n";
	/*for (const auto& monster : m_AreaMonsters)
	{
		monster->SetIsActive(true);
	}
	g_Room->HandleOpenGate(m_GateInfo);
	g_Room->RemoveObject((uint32)EObject_Type::TRIGGER, ObjectInfo->object_id());*/
}

void CMonseterTriggerBox::CollisionEnd(CBoxCollider* src, CBoxCollider* dest)
{
	CMonsterArea* area = new CMonsterArea();
	for (const auto& monster : m_AreaMonsters)
	{
		monster->SetIsActive(true);
		area->PushMonsterId(monster->MonsterInfo->object_id());
	}
	g_Room->GetAreas().push_back(area);
	g_Room->HandleOpenGate(m_GateInfo);
	g_Room->RemoveObject((uint32)EObject_Type::TRIGGER, ObjectInfo->object_id());
}
