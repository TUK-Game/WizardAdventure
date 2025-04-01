#include "pch.h"
#include "Player.h"

CPlayer::CPlayer()
{
	m_bPlayer = true;
	ObjectInfo = new Protocol::ObjectInfo();
	PosInfo = new Protocol::PosInfo();
	PlayerInfo = new Protocol::PlayerInfo();
	ObjectInfo->set_allocated_pos_info(PosInfo);
	PlayerInfo->set_allocated_object_info(ObjectInfo);
}

CPlayer::~CPlayer()
{
}
