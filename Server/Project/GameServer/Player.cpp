#include "pch.h"
#include "Player.h"

CPlayer::CPlayer()
{
	m_bPlayer = true;
	ObjectInfo = new Protocol::ObjectInfo();
	PosInfo = new Protocol::PosInfo();
	PlayerInfo = new Protocol::PlayerInfo();
	ObjectInfo->set_allocated_pos_info(PosInfo);
	PosInfo = new Protocol::PosInfo();
	PlayerInfo->set_allocated_object_info(ObjectInfo);
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
