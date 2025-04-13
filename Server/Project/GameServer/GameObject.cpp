#include "pch.h"
#include "GameObject.h"
#include "BoxCollider.h"

CGameObject::CGameObject()
	: m_bPlayer(false)
{
	ObjectInfo = new Protocol::ObjectInfo();
	PosInfo = new Protocol::PosInfo();
	ObjectInfo->set_allocated_pos_info(PosInfo);

	m_BoxCollider = new CBoxCollider;
	m_BoxCollider->SetOwner(this);
}

CGameObject::~CGameObject()
{
	if(ObjectInfo)	
		delete ObjectInfo;
	if (PosInfo)
		delete PosInfo;
	if (m_BoxCollider)
		delete m_BoxCollider;
}

void CGameObject::Update(float deltaTime)
{
	if(m_BoxCollider)
	{
		m_BoxCollider->Update();
	}
}
