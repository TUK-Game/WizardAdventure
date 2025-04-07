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
	if (m_State == Protocol::MOVE_STATE_DASH)
	{
		float deltaTime = g_Timer->GetDeltaTime();
		m_DashElapsedTime += deltaTime;
		if (m_DashElapsedTime >= m_DashDurtation)
		{
			m_State = Protocol::MOVE_STATE_DASH_END;
			m_DashElapsedTime = 0;
		}
		else
		{
			//XMVECTOR dir = DirectX::XMVector3Normalize(DirectX::XMVectorSet(m_Dir.x(), m_Dir.y(), m_Dir.z(), 0.0f));

			XMFLOAT3 xdir;
			//XMStoreFloat3(&xdir, dir);
			m_NextAmount.set_x(m_Dir.x() * m_DashDurtation * deltaTime * m_Speed);
			m_NextAmount.set_y(m_Dir.y() * m_DashDurtation * deltaTime * m_Speed);
			m_NextAmount.set_z(m_Dir.z() * m_DashDurtation * deltaTime * m_Speed);
			std::cout << m_NextAmount.x() << " " << m_NextAmount.y() << " " << m_NextAmount.z() << '\n';
		}
		g_Room->HandleMovePlayer(m_Session.lock()->Player);
	}
}

void CPlayer::CollisionBegin(CBoxCollider* src, CBoxCollider* dest)
{
	std::cout << "³ª ¹ÚÀ½" << std::endl;
}

void CPlayer::CollisionEvent(CBoxCollider* src, CBoxCollider* dest)
{
}
