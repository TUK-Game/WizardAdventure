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
	// ID ����
	const int64 newId = s_IdGenerator.fetch_add(1);

	CPlayerRef player = std::make_shared<CPlayer>();

	// object_id�� ��� ������Ʈ���� �߱޵Ǵ� ���̵�.
	// ��� �÷��̾����� �����ϱ� ���ؼ��� session_id�� �ʿ���. session Ŭ������ id�� ���� �� �� ����.
	// ���� ������� object_id �� player_id(session_id) �� �� ���� ����� �����Ͽ��� ��.
	player->PlayerInfo->set_player_id(session->GetId());
	player->ObjectInfo->set_object_id(newId);
	player->PosInfo->set_object_id(newId);
	player->m_Session = session;
	session->Player.store(player);

	return player;
}

CGameObjectRef CObjectUtil::CreateObject()
{
	// ID ����
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
