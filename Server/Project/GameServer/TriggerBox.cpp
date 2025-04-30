#include "pch.h"
#include "TriggerBox.h"
#include "BoxCollider.h"
#include "Room.h"
#include "LevelCollision.h"

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

void CTriggerBox::SetTriggerBox(const Vec3& pos, const Vec3& size)
{
	ObjectInfo->mutable_pos_info()->mutable_position()->set_x(pos.x);
	ObjectInfo->mutable_pos_info()->mutable_position()->set_y(pos.y);
	ObjectInfo->mutable_pos_info()->mutable_position()->set_z(pos.z);

	m_BoxCollider->SetBoxInfo(pos, size, Vec3(0.f, 0.f, 0.f));
}

void CTriggerBox::Update(float deltaTime)
{
	g_Room->GetLevelCollision()->AddCollider(m_BoxCollider, ECollision_Channel::TRIGGER);
}

void CTriggerBox::CollisionBegin(CBoxCollider* src, CBoxCollider* dest)
{
}

void CTriggerBox::CollisionEnd(CBoxCollider* src, CBoxCollider* dest)
{
}
