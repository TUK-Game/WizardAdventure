#include "pch.h"
#include "TriggerBox.h"
#include "BoxCollider.h"
#include "Room.h"
#include "LevelCollision.h"
#include "Monster.h"

CTriggerBox::CTriggerBox()
{
	ObjectInfo = new Protocol::ObjectInfo();
	PosInfo = new Protocol::PosInfo();
	ObjectInfo->set_allocated_pos_info(PosInfo);

	m_BoxCollider = new CBoxCollider;
	m_BoxCollider->SetOwner(this);
	m_BoxCollider->SetCollisionProfile("TRIGGER");
}

CTriggerBox::~CTriggerBox()
{
}

void CTriggerBox::Init()
{

}

void CTriggerBox::SetTriggerBox(const Vec3& pos, const Vec3& size)
{
	ObjectInfo->mutable_pos_info()->mutable_position()->set_x(pos.x);
	ObjectInfo->mutable_pos_info()->mutable_position()->set_y(pos.y);
	ObjectInfo->mutable_pos_info()->mutable_position()->set_z(pos.z);

	m_BoxCollider->SetBoxInfo(pos, size, Vec3(0.f, 0.f, 0.f));
}

bool CTriggerBox::IsMonsterInArea(const Vec3& pos)
{
	Vec3 min = m_AreaCenter - (m_AreaSize * 0.5f);
	Vec3 max = m_AreaCenter + (m_AreaSize * 0.5f);

	return (pos.x >= min.x && pos.x <= max.x && pos.z >= min.z && pos.z <= max.z);
}

void CTriggerBox::Update(float deltaTime)
{
	g_Room->GetLevelCollision()->AddCollider(m_BoxCollider, ECollision_Channel::TRIGGER);
}

void CTriggerBox::CollisionBegin(CBoxCollider* src, CBoxCollider* dest)
{
	//std::cout << "트리거 박스 발동\n";
	/*for (const auto& monster : m_AreaMonsters)
	{
		monster->SetIsActive(true);
	}
	g_Room->HandleOpenGate(m_GateInfo);
	g_Room->RemoveObject((uint32)EObject_Type::TRIGGER, ObjectInfo->object_id());*/
}

void CTriggerBox::CollisionEnd(CBoxCollider* src, CBoxCollider* dest)
{
	for (const auto& monster : m_AreaMonsters)
	{
		monster->SetIsActive(true);
	}
	g_Room->HandleOpenGate(m_GateInfo);
	g_Room->RemoveObject((uint32)EObject_Type::TRIGGER, ObjectInfo->object_id());
}
