#include "pch.h"
#include "NPC.h"
#include "BoxCollider.h"

CNPC::CNPC()
{
	ObjectInfo = new Protocol::ObjectInfo();
	PosInfo = new Protocol::PosInfo();

	ObjectInfo->set_allocated_pos_info(PosInfo);

	m_BoxCollider = new CBoxCollider();
	m_BoxCollider->SetOwner(this);

}

CNPC::~CNPC()
{
}
