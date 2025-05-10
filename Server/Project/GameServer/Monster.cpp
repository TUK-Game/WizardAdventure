#include "pch.h"
#include "Monster.h"
#include "CreatureCollider.h"
#include "MonsterAI.h"
#include "MonsterCollider.h"
#include "Room.h"
#include "Projectile.h"
#include "Player.h"
#include "ProjectilePool.h"
#include "Projectile.h"

CMonster::CMonster()
{
	if (ObjectInfo)
	{
		delete ObjectInfo;
	}

	ObjectInfo = new Protocol::ObjectInfo();
	PosInfo = new Protocol::PosInfo();
	MonsterInfo = new Protocol::MonsterInfo();

	ObjectInfo->set_allocated_pos_info(PosInfo);
	MonsterInfo->set_allocated_object_info(ObjectInfo);

	m_BoxCollider = new CMonsterCollider();
	m_BoxCollider->SetOwner(this);

	m_Ai = new CMonsterAI();
	m_Ai->SetOwner(this);
}

CMonster::~CMonster()
{
	delete MonsterInfo;
	delete m_Ai;
}

Vec3 CMonster::GetFrontVec()
{
	m_Dir.Normalize();
	std::cout << "DIR: " << m_Dir.x << " " << m_Dir.y << " " << m_Dir.z << '\n';
	return m_Dir;
}

void CMonster::SetDir(float y)
{
	float rad = XMConvertToRadians(y);
	m_Dir = Vec3(-sinf(rad), 0.f, -cosf(rad));
}

void CMonster::Update(float deltaTime)
{
	if (m_bActive)
	{
		CGameObject::Update(deltaTime);
		m_Ai->Update(deltaTime);
	}
}

void CMonster::CollisionBegin(CBoxCollider* src, CBoxCollider* dest)
{
	if (GetAbility()->currentHp <= 0)
		return;

	if (dest->GetProfile()->channel == ECollision_Channel::Projectile &&
		Protocol::MOVE_STATE_NONE != m_State)
	{
		if (GetAbility()->currentHp > 0)
		{
			Damaged((dynamic_cast<CProjectile*>(dest->GetOwner()))->GetAttack());
			m_State = Protocol::MOVE_STATE_DAMAGED;			// damage state
			if (GetAbility()->currentHp <= 0)
			{
				std::cout << "Á×À½ »óÅÂ\n";
				m_State = Protocol::MOVE_STATE_DEATH;		// death state

				const auto& players = g_Room->GetPlayers();
				for (const auto& player : players)
				{
					if (!player) continue;
					player->GetAbility()->gold += GetAbility()->gold;
				}
			}
		}
	}
}

void CMonster::CollisionEvent(CBoxCollider* src, CBoxCollider* dest)
{
}

void CMonster::SpawnAttackObject()
{
	CProjectileRef projectile = g_pool->Allocate();
	assert(projectile != nullptr);

	projectile->GetCollider()->SetCollisionProfile("MonsterProjectile");

	ProjectileState state;
	state.Direction = GetFrontVec();
	state.Size = Vec3(200.f, 200.f, 200.f);
	state.Speed = 0.f;
	//state.ElapsedTime = info.duration();
	state.damage = GetAbility()->attack;

	const auto& pos = MonsterInfo->object_info().pos_info().position();
	Vec3 p = Vec3(pos.x(), pos.y(), pos.z());
	Vec3 spawnPos = p + state.Direction * 400.f;

	projectile->SetProjectileState(state);
	projectile->SetProjectileState(state);
	projectile->SetCollisionExplosion(false);
	projectile->ProjectileInfo->mutable_object_info()->mutable_pos_info()->mutable_position()->set_x(spawnPos.x);
	projectile->ProjectileInfo->mutable_object_info()->mutable_pos_info()->mutable_position()->set_y(spawnPos.y);
	projectile->ProjectileInfo->mutable_object_info()->mutable_pos_info()->mutable_position()->set_z(spawnPos.z);
	projectile->ProjectileInfo->mutable_object_info()->mutable_pos_info()->mutable_size()->set_x(state.Size.x);
	projectile->ProjectileInfo->mutable_object_info()->mutable_pos_info()->mutable_size()->set_y(state.Size.y);
	projectile->ProjectileInfo->mutable_object_info()->mutable_pos_info()->mutable_size()->set_z(state.Size.z);
	projectile->ProjectileInfo->set_state(Protocol::MOVE_STATE);
	projectile->SetMeshSize(Vec3(1.f, 1.f, 1.f));
	projectile->SetCollisionBoxInfo(spawnPos, state.Size * projectile->GetMeshSize(), Vec3(0.f, 0.f, 0.f));
	//projectile->m_meshType = pkt.mesh();
	g_Room->AddProjectile(projectile);
	m_ProjectileIds.push_back(projectile->ProjectileInfo->projectile_id());
}
