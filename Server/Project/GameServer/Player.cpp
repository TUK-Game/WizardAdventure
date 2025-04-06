#include "pch.h"
#include "Player.h"
#include "CreatureCollider.h"
#include "Room.h"

CPlayer::CPlayer()
{
	m_bPlayer = true;
	ObjectInfo = new Protocol::ObjectInfo();
	PosInfo = new Protocol::PosInfo();
	PlayerInfo = new Protocol::PlayerInfo();
	ObjectInfo->set_allocated_pos_info(PosInfo);
	PlayerInfo->set_allocated_object_info(ObjectInfo);

	m_BoxCollider = new CCreatureCollider();
	m_BoxCollider->SetOwner(this);
}

CPlayer::~CPlayer()
{
}

void CPlayer::Update()
{
	CGameObject::Update();
}

void CPlayer::CollisionBegin(CBoxCollider* src, CBoxCollider* dest)
{
	std::cout << "³ª ¹ÚÀ½" << std::endl;
}

void CPlayer::CollisionEvent(CBoxCollider* src, CBoxCollider* dest)
{
	//{

	//	XMFLOAT3 dc = dest->GetBoundingBox().Center;
	//	XMFLOAT3 de = dest->GetBoundingBox().Extents;

	//	XMFLOAT3 sc = src->GetBoundingBox().Center;
	//	XMFLOAT3 se = src->GetBoundingBox().Extents;

	//	Protocol::Vector3* pos = new Protocol::Vector3;

	//	pos->CopyFrom(PlayerInfo->object_info().pos_info().position());

	//	float dx = sc.x - dc.x;
	//	float dz = sc.z - dc.z;

	//	float px = (de.x + se.x) - abs(dx);
	//	float pz = (de.z + se.z) - abs(dz);

	//	const float margin = 40.f;
	//	if (px < pz)
	//	{
	//		if (dx > 0)
	//		{
	//			pos->set_x(dc.x + de.x + se.x + margin);
	//			block[0] = true;
	//		}
	//		else
	//		{
	//			pos->set_x(dc.x - de.x - se.x - margin);
	//			block[1] = true;
	//		}
	//	}
	//	else
	//	{
	//		if (dz > 0)
	//		{
	//			pos->set_z(dc.z + de.z + se.z + margin);
	//			block[2] = true;
	//		}
	//		else
	//		{
	//			pos->set_z(dc.z - de.z - se.z - margin);
	//			block[3] = true;
	//		}
	//	}

	//	PlayerInfo->mutable_object_info()->mutable_pos_info()->set_allocated_position(pos);


	//	CPlayerRef player = m_Session.lock()->Player.load();
	//	g_Room->DoAsync(&CRoom::HandleMovePlayer, player);
	//}
}
