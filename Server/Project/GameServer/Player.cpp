#include "pch.h"
#include "Player.h"
#include "CreatureCollider.h"
#include "Room.h"
#include "Monster.h"
#include "Item.h"
#include "Skill.h"
#include "Projectile.h"

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

bool CPlayer::BuyItem(CItemRef item)
{
	auto iter = std::find_if(m_Items.begin(), m_Items.end(), [&](CItemRef tem) {
		return tem->GetItemInfo().id == item->GetItemInfo().id;
		});

	if (iter != m_Items.end())
		return false;

	float price = item->GetItemInfo().price;
	//if (price > GetAbility()->gold)
	//	return false;

	GetAbility()->gold -= price;
	if (CalculateAbility(item))
	{
		m_Items.emplace_back(item);
	}
	g_Room->UpdatePlayerAbility(m_Session.lock()->Player);
	return true;
}

bool CPlayer::BuySkill(CSkillRef skill)
{
	auto iter = std::find_if(m_Skills.begin(), m_Skills.end(), [&](CSkillRef s) {
		return s->GetSkillInfo().id == skill->GetSkillInfo().id;
		});

	if (iter != m_Skills.end())
		return false;

	float price = skill->GetSkillInfo().price;
	//if (price > GetAbility()->gold)
	//	return false;

	GetAbility()->gold -= price;

	m_Skills.emplace_back(skill);
	g_Room->UpdatePlayerAbility(m_Session.lock()->Player);
	return true;
}

bool CPlayer::CalculateAbility(CItemRef item)
{
	const auto& info = item->GetItemInfo();
	switch (info.part)
	{
	case EITEM_PART::ATTACK:
	{
		GetAbility()->attack += info.amount;
	}
	break;
	case EITEM_PART::MAXHP:
	{
		GetAbility()->maxHp += info.amount;
		GetAbility()->currentHp += info.amount;
	}
	break;
	case EITEM_PART::SPEED:
	{
		GetAbility()->moveSpeed += info.amount;
	}
	break;
	case EITEM_PART::HEAL:
	{
		GetAbility()->currentHp = (std::min)((int)info.amount + GetAbility()->currentHp, GetAbility()->maxHp);
		return false;
	}
	break;
	}
	return true;
}

void CPlayer::Update(float deltaTime)
{
	CGameObject::Update(deltaTime);

	if (m_bDamageDelay)
	{
		m_DamageDelayElapsedTime += deltaTime;
		std::cout << m_DamageDelayElapsedTime << '\n';
		std::cout << "¹«Àû\n";
		if (m_DamageDelayElapsedTime >= m_DamageDelayDuration)
		{
			m_DamageDelayElapsedTime = 0;
			m_State = Protocol::MOVE_STATE_DAMAGE_DELAY_END;
			g_Room->UpdatePlayerState(m_Session.lock()->Player);
			m_bDamageDelay = false;
		}
	}

	if (m_State == Protocol::MOVE_STATE_DASH)
	{
		m_DashElapsedTime += deltaTime;
		if (m_DashElapsedTime >= m_DashDuration)
		{
			m_State = Protocol::MOVE_STATE_DASH_END;
			g_Room->UpdatePlayerState(m_Session.lock()->Player);
			m_DashElapsedTime = 0;
		}
		else
		{
			float scale = deltaTime * m_Speed * 0.2;
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
	else if (m_State == Protocol::MOVE_STATE_DAMAGED)
	{
		m_DamageElapsedTime += deltaTime;
		if (m_DamageElapsedTime >= m_DamageDuration)
		{
			m_State = Protocol::MOVE_STATE_DAMAGE_DELAY;
			g_Room->UpdatePlayerState(m_Session.lock()->Player);
			//m_State = Protocol::MOVE_STATE_DAMAGED_END;
			m_DamageElapsedTime = 0;
			m_bDamageDelay = true;
		}
		else
		{
			//float scale = m_DamageDuration * deltaTime * m_Speed * -1;
			Vec3 dir{ m_Dir.x(), m_Dir.y(), m_Dir.z() };
			dir.Normalize();
			// 0.1 -> speedscale
			dir *= deltaTime * m_Speed * -0.1f;
			m_NextAmount.set_x(dir.x);
			m_NextAmount.set_y(dir.y);
			m_NextAmount.set_z(dir.z);
		}
		g_Room->HandleMovePlayer(m_Session.lock()->Player);
	}
	else if (m_State == Protocol::MOVE_STATE_DEATH)
	{
		m_DeathElapsedTime += deltaTime;
		if (m_DeathElapsedTime + 0.1f >= m_DeathDuration)
		{
			m_State = Protocol::MOVE_STATE_DEATH_END;
			g_Room->UpdatePlayerState(m_Session.lock()->Player);
			m_DeathElapsedTime = 0;
		}
	}
}

void CPlayer::CollisionBegin(CBoxCollider* src, CBoxCollider* dest)
{
	if (GetAbility()->currentHp <= 0 || m_bDamageDelay || m_State == Protocol::MOVE_STATE_DAMAGED)
		return;

	if (dest->GetProfile()->channel == ECollision_Channel::MonsterProjectile || dest->GetProfile()->channel == ECollision_Channel::Monster)
	{
		if (dest->GetProfile()->channel == ECollision_Channel::MonsterProjectile)
		{
			CProjectile* projectile = (dynamic_cast<CProjectile*>(dest->GetOwner()));
			GetAbility()->currentHp -= projectile->GetAttack();
		}
		else if (dest->GetProfile()->channel == ECollision_Channel::Monster)
		{
			CMonster* monster = (dynamic_cast<CMonster*>(dest->GetOwner()));

			if (monster->GetAbility()->currentHp <= 0)
				return;
			GetAbility()->currentHp -= monster->GetAbility()->attack;
		}

		if (GetAbility()->currentHp <= 0)
		{
			SetState(Protocol::MOVE_STATE_DEATH);
			std::cout << "Á×À½\n";
		}
		else
		{
			SetState(Protocol::MOVE_STATE_DAMAGED);
		}
		g_Room->UpdatePlayerAbility(m_Session.lock()->Player);
		g_Room->UpdatePlayerState(m_Session.lock()->Player);
	}
}

void CPlayer::CollisionEvent(CBoxCollider* src, CBoxCollider* dest)
{
}
