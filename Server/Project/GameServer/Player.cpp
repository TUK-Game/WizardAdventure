#include "pch.h"
#include "Player.h"
#include "CreatureCollider.h"
#include "Room.h"
#include "Monster.h"

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
	delete PlayerInfo;
}

void CPlayer::Update(float deltaTime)
{
	CGameObject::Update(deltaTime);
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
			float scale = m_DashDurtation * deltaTime * m_Speed;
			XMFLOAT3 scaledDir =
			{
				m_Dir.x() * scale,
				m_Dir.y() * scale,
				m_Dir.z() * scale
			};

			ToProtoVector3(&m_NextAmount, scaledDir);
		}
		g_Room->HandleMovePlayer(m_Session.lock()->Player);
	}
	else
	{
		if (PlayerInfo->object_info().pos_info().position().y() <= -200.f)
		{
			const auto& pos = PlayerInfo->mutable_object_info()->mutable_pos_info()->mutable_position();
			pos->set_x(m_SafePos.x);
			pos->set_y(m_SafePos.y);
			pos->set_z(m_SafePos.z);
		}
	}
}

void CPlayer::CollisionBegin(CBoxCollider* src, CBoxCollider* dest)
{
	if (dest->GetProfile()->channel == ECollision_Channel::Monster)
	{
		CMonster* monster = (dynamic_cast<CMonster*>(dest->GetOwner()));
		GetAblity()->currentHp -= monster->GetAblity()->attack;
	}

}

void CPlayer::CollisionEvent(CBoxCollider* src, CBoxCollider* dest)
{
}
