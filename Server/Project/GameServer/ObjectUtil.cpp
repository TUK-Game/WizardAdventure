#include "pch.h"
#include "ObjectUtil.h"
#include "Player.h"
#include "GameSession.h"
#include "Monster.h"
#include "Projectile.h"
#include "MonsterTriggerBox.h"

std::atomic<int64>	CObjectUtil::s_IdGenerator = 1;

CPlayerRef CObjectUtil::CreatePlayer(CGameSessionRef session)
{
	// ID 생성
	const int64 newId = s_IdGenerator.fetch_add(1);

	CPlayerRef player = std::make_shared<CPlayer>();

	// object_id는 모든 오브젝트에게 발급되는 아이디.
	// 몇번 플레이어인지 구분하기 위해서는 session_id가 필요함. session 클래스의 id를 보면 알 수 있음.
	// 따라서 원래라면 object_id 와 player_id(session_id) 를 두 개다 만들고 관리하여야 함.
	player->PlayerInfo->set_player_id(session->GetId());
	player->ObjectInfo->set_object_id(newId);
	player->PosInfo->set_object_id(newId);
	player->m_Session = session;
	session->Player.store(player);

	return player;
}

CGameObjectRef CObjectUtil::CreateObject()
{
	// ID 생성
	const int64 newId = s_IdGenerator.fetch_add(1);

	CGameObjectRef object = std::make_shared<CGameObject>();

	object->ObjectInfo->set_object_id(newId);
	object->PosInfo->set_object_id(newId);

	return object;
}

CMonsterRef CObjectUtil::CreateMonster()
{
	const int64 newId = s_IdGenerator.fetch_add(1);

	CMonsterRef monster = std::make_shared<CMonster>();

	monster->MonsterInfo->set_object_id(newId);

	return monster;
}

CProjectileRef CObjectUtil::CreateProjectile()
{
	const int64 newId = s_IdGenerator.fetch_add(1);

	CProjectileRef projectile = std::make_shared<CProjectile>();

	projectile->ProjectileInfo->set_projectile_id(newId);

	return projectile;
}
