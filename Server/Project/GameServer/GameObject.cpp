#include "pch.h"
#include "GameObject.h"

CGameObject::CGameObject()
	: m_bPlayer(false)
{
	ObjectInfo = new Protocol::ObjectInfo();
	PosInfo = new Protocol::PosInfo();
	ObjectInfo->set_allocated_pos_info(PosInfo);
}

CGameObject::~CGameObject()
{
	delete ObjectInfo;
}
