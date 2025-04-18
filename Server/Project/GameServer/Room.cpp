#include "pch.h"
#include "Room.h"
#include "Player.h"
#include "GameSession.h"
#include "Monster.h"
#include "ObjectUtil.h"
#include "LevelCollision.h"
#include "BoxCollider.h"
#include "Projectile.h"
#include "ProjectilePool.h"

CRoomRef g_Room = std::make_shared<CRoom>();

void ToProtoVector3(Protocol::Vector3* to, const XMFLOAT3& from)
{
	to->set_x(from.x);
	to->set_y(from.y);
	to->set_z(from.z);
}

XMFLOAT3 ProtoToXMFLOAT3(const Protocol::Vector3& from)
{
	return XMFLOAT3(from.x(), from.y(), from.z());
}

CRoom::CRoom()
{
	m_LevelCollision = new CLevelCollision();
}

CRoom::~CRoom()
{
	if(m_LevelCollision)
	{
		delete m_LevelCollision;
		m_LevelCollision = nullptr;
	}
}

CRoomRef CRoom::GetRoomRef()
{
	return static_pointer_cast<CRoom>(shared_from_this());
}

void CRoom::Init()
{
}

void CRoom::Update()
{
	//std::cout << "Update Room" << std::endl;
	g_Timer->Update();
	m_DeltaTime = g_Timer->GetDeltaTime();

	for (const auto& layer : m_mapObject)
	{
		for (const auto object : layer)
		{
			auto& gameObject = object.second;
			if(gameObject)
			{
				gameObject->Update(m_DeltaTime);
			}
		}
	}

	for (const auto& player : m_Players)
	{
		if(player)
			player->Update(m_DeltaTime);
	}

	m_LevelCollision->Collision();

	// ���� ���� ����
	UpdateClients();

	// 33ms ���� ���� ����
	// => ���� 30������
	DoTimer(33, &CRoom::Update);
}

void CRoom::UpdateClients()
{
	UpdateMonster();
}

void CRoom::UpdateMonster()
{
	std::unordered_map<uint64, CGameObjectRef> monsters = GetLayerObjects((int)EObject_Type::Monster);

	Protocol::S_MONSTER_INFO pkt;
	for (const auto& pair : monsters)
	{
		CMonster* monster = (CMonster*)(pair.second.get());
		Protocol::MonsterInfo* info = pkt.add_monster_info();

		info->set_object_id(monster->MonsterInfo->object_id());
		info->set_monster_type(monster->MonsterInfo->monster_type());

		const Protocol::PosInfo& srcPosInfo = monster->MonsterInfo->object_info().pos_info();

		Protocol::PosInfo* destPosInfo = info->mutable_object_info()->mutable_pos_info();

		Protocol::Vector3* pos = destPosInfo->mutable_position();
		pos->set_x(srcPosInfo.position().x());
		pos->set_y(srcPosInfo.position().y());
		pos->set_z(srcPosInfo.position().z());

		Protocol::Vector3* rot = destPosInfo->mutable_rotation();
		rot->set_x(srcPosInfo.rotation().x());
		rot->set_y(srcPosInfo.rotation().y());
		rot->set_z(srcPosInfo.rotation().z());

		destPosInfo->set_state(monster->GetState());
	}
	CSendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(pkt);
	Broadcast(sendBuffer, -1);
}

void CRoom::UpdateProjectile()
{
	std::unordered_map<uint64, CGameObjectRef> projectiles = GetLayerObjects((int)EObject_Type::Projectile);
	Protocol::S_PROJECTILE_INFO pkt;
	for (const auto& pair : projectiles)
	{

	}
}

bool CRoom::EnterRoom(CPlayerRef newPlayer, bool bRandPos /*= true*/)
{
	bool success = AddPlayer(newPlayer);

	if (success)
	{
		std::cout << "Enter Player " << newPlayer->PlayerInfo->player_id() << std::endl;
	}

	Protocol::Vector3* position = new Protocol::Vector3();
	position->set_x(11240.f);
	position->set_y(20.f);
	position->set_z(1127.f);

	newPlayer->PlayerInfo->mutable_object_info()->mutable_pos_info()->set_allocated_position(position);
	newPlayer->GetCollider()->SetCollisionProfile("Player");
	newPlayer->GetCollider()->SetBoxInfo(XMFLOAT3(11240.f, 20.f, 1127.f), XMFLOAT3(100.f, 200.f, 24.f), XMFLOAT3(0.f, 0.f, 0.f), XMFLOAT3(0.f, 100.f, 0.f));

	// ���� ����� �� �÷��̾�� �˸���
	{
		Protocol::S_ENTER_GAME enterGamePkt;
		enterGamePkt.set_success(success);

		Protocol::PlayerInfo* playerInfo = new Protocol::PlayerInfo();
		playerInfo->CopyFrom(*newPlayer->PlayerInfo);
		enterGamePkt.set_allocated_player(playerInfo);	

		CSendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(enterGamePkt);
		if (auto session = newPlayer->GetSession())
			session->Send(sendBuffer);
	}

	// ���� ����� �ٸ� �÷��̾�� �˸���
	{
		Protocol::S_SPAWN_NEW_PLAYER spawnPkt;

		Protocol::PlayerInfo* playerInfo = spawnPkt.mutable_player();
		playerInfo->CopyFrom(*(newPlayer->PlayerInfo));

		CSendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(spawnPkt);
		Broadcast(sendBuffer, newPlayer->PlayerInfo->player_id());
	}

	// ������ �÷��̾� ������ �� �÷��̾�� �˸���
	{
		Protocol::S_SPAWN_EXISTING_PLAYER spawnPkt;

		for (auto& player : m_Players)
		{
			if (player == nullptr)
				continue;
			if (player->PlayerInfo->player_id() == newPlayer->PlayerInfo->player_id())
				continue;

			Protocol::PlayerInfo* playerInfo = spawnPkt.add_player();
			playerInfo->CopyFrom(*player->PlayerInfo);
		}

		CSendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(spawnPkt);
		if (auto session = newPlayer->GetSession())
			session->Send(sendBuffer);
	}

	return success;
}

bool CRoom::LeaveRoom(CPlayerRef leavePlayer)
{
	if (leavePlayer == nullptr)
		return false;

	const uint64 playerId = leavePlayer->PlayerInfo->player_id();
	bool success = RemovePlayer(playerId);

	if (success)
	{
		std::cout << "Leave Player " << playerId << std::endl;
	}

	// ���� ����� �����ϴ� �÷��̾�� �˸���
	{
		Protocol::S_LEAVE_GAME leaveGamePkt;

		CSendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(leaveGamePkt);
		if (auto session = leavePlayer->GetSession())
			session->Send(sendBuffer);
	}

	// ���� ����� �˸���
	{
		Protocol::S_DESPAWN_PLAYER despawnPkt;	
		despawnPkt.set_player_ids(playerId);

		CSendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(despawnPkt);
		Broadcast(sendBuffer, playerId);

	/*	if (auto session = leavePlayer->GetSession())
			session->Send(sendBuffer);*/
	}

	return success;
}

bool CRoom::HandleEnterPlayer(CPlayerRef player)
{
	return EnterRoom(player, true);
}

bool CRoom::HandleLeavePlayer(CPlayerRef player)
{
	return LeaveRoom(player);
}

bool CRoom::HandleMovePlayer(CPlayerRef player)
{
	// 1. �浹üũ
	// 2. ��ġ����
	// 3. ��� Ŭ�� ��ġ ����
	int step = 1;

	Protocol::Vector3& protoNow = *player->PlayerInfo->mutable_object_info()->mutable_pos_info()->mutable_position();
	XMFLOAT3 nowPos(protoNow.x(), protoNow.y(), protoNow.z());
	// �̵���
	XMFLOAT3 moveAmount = ProtoToXMFLOAT3(player->m_NextAmount);

	moveAmount.x /= static_cast<float>(step);
	moveAmount.y /= static_cast<float>(step);
	moveAmount.z /= static_cast<float>(step);

	for (int i = 1; i <= step; ++i)
	{
		nowPos.x += moveAmount.x;
		nowPos.y += moveAmount.y;
		nowPos.z += moveAmount.z;

		ToProtoVector3(&protoNow, nowPos);

		player->GetCollider()->Update();

		if (m_LevelCollision->CollisionWithWall(player->GetCollider()))
		{
			std::cout << "����" << std::endl;
			Protocol::Vector3& dir = player->GetDir();
			const float ratio = 1.1f;
			nowPos.x -= moveAmount.x;
			nowPos.y -= moveAmount.y;
			nowPos.z -= moveAmount.z;

			XMVECTOR dirVec = XMVectorSet(dir.x(), dir.y(), dir.z(), 0.0f);
			dirVec = XMVector3Normalize(dirVec);
			if(player->GetState() != Protocol::MOVE_STATE_DASH && player->GetState() != Protocol::MOVE_STATE_DASH_END)
				dirVec = XMVectorScale(dirVec, -20);

			XMVECTOR now = XMLoadFloat3(&nowPos);
			now = XMVectorSubtract(now, dirVec);
			XMStoreFloat3(&nowPos, now);

			ToProtoVector3(&protoNow, nowPos);
			break;
		}
	}

	Protocol::S_MOVE movePkt;
	auto* moveInfo = movePkt.mutable_player_move_info();
	moveInfo->set_player_id(player->PlayerInfo->player_id());

	auto* posInfo = moveInfo->mutable_pos_info();
	ToProtoVector3(posInfo->mutable_position(), nowPos);

	const auto& rot = player->PlayerInfo->object_info().pos_info().rotation();
	ToProtoVector3(posInfo->mutable_rotation(), XMFLOAT3(rot.x(), rot.y(), rot.z()));

	posInfo->set_state(player->GetState());

	std::cout << "���� " << nowPos.x << " " << nowPos.y << " " << nowPos.z << '\n';

	CSendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(movePkt);
	Broadcast(sendBuffer, player->PlayerInfo->player_id());

	if (auto session = player->GetSession())
		session->Send(sendBuffer);

	return true;
}

bool CRoom::HandleSpawnProjectile(CProjectileRef projectile)
{
	AddProjectile(projectile);

	Protocol::S_SPAWN_PROJECTILE_SUCESSE pkt;
	pkt.set_projectile_id(projectile->ProjectileInfo->projectile_id());

	CSendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(pkt);
	Broadcast(sendBuffer, -1);
	return true;
}

bool CRoom::HandleMoveProjectile(CProjectileRef projectile)
{
	Protocol::S_PROJECTILE_INFO pkt;
	if (projectile->ProjectileInfo->state() == Protocol::COLLISION)
	{
		pkt.mutable_projectile_info()->set_projectile_id(projectile->ProjectileInfo->projectile_id());
		pkt.mutable_projectile_info()->set_state(Protocol::COLLISION);
		m_mapObject[(uint32)EObject_Type::Projectile].erase(projectile->ProjectileInfo->projectile_id());
	}
	else
	{
		int step = 1;

		Protocol::Vector3& protoNow = *projectile->ProjectileInfo->mutable_object_info()->mutable_pos_info()->mutable_position();
		XMFLOAT3 nowPos(protoNow.x(), protoNow.y(), protoNow.z());
		// �̵���
		XMFLOAT3 moveAmount = projectile->m_NextAmount;

		moveAmount.x /= static_cast<float>(step);
		moveAmount.y /= static_cast<float>(step);
		moveAmount.z /= static_cast<float>(step);

		for (int i = 1; i <= step; ++i)
		{
			nowPos.x += moveAmount.x;
			nowPos.y += moveAmount.y;
			nowPos.z += moveAmount.z;

			ToProtoVector3(&protoNow, nowPos);

			projectile->GetCollider()->Update();

			if (m_LevelCollision->CollisionWithWall(projectile->GetCollider()))
			{
				std::cout << "����" << std::endl;
				projectile->ProjectileInfo->set_state(Protocol::COLLISION);
				m_mapObject[(uint32)EObject_Type::Projectile].erase(projectile->ProjectileInfo->projectile_id());
				g_pool->Release(projectile);
				break;
			}
		}


		auto* info = pkt.mutable_projectile_info();
		info->set_projectile_id(projectile->ProjectileInfo->projectile_id());
		info->set_state(projectile->ProjectileInfo->state());

		auto* posInfo = pkt.mutable_projectile_info()->mutable_object_info()->mutable_pos_info();
		posInfo->mutable_position()->set_x(nowPos.x);
		posInfo->mutable_position()->set_y(nowPos.y);
		posInfo->mutable_position()->set_z(nowPos.z);
	}
	CSendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(pkt);
	Broadcast(sendBuffer, -1);

	//if (auto session = player->GetSession())
	//	session->Send(sendBuffer);

	return true;
}

bool CRoom::AddPlayer(CPlayerRef player)
{
	if (m_Players[player->PlayerInfo->player_id()])
		return false;

	m_Players[player->PlayerInfo->player_id()] = player;
	player->m_Room.store(GetRoomRef());
	return true;
}

bool CRoom::AddObject(uint32 layer, CGameObjectRef object)
{
	if (m_mapObject[layer].find(object->ObjectInfo->object_id()) != m_mapObject[layer].end())
		return false;

	m_mapObject[layer].insert(make_pair(object->ObjectInfo->object_id(), object));

	object->m_Room.store(GetRoomRef());

	return true;
}

bool CRoom::AddProjectile(CProjectileRef object)
{
	if (m_mapObject[(uint32)EObject_Type::Projectile].find(object->ProjectileInfo->projectile_id()) != m_mapObject[(uint32)EObject_Type::Projectile].end())
		return false;

	m_mapObject[(uint32)EObject_Type::Projectile].insert(make_pair(object->ProjectileInfo->projectile_id(), object));

	object->m_Room.store(GetRoomRef());

	return true;
}

bool CRoom::RemovePlayer(uint64 playerId)
{
	if (m_Players[playerId] == nullptr)
		return false;

	CPlayerRef player = m_Players[playerId];
	if (player)
		player->m_Room.store(std::weak_ptr<CRoom>());

	m_Players[playerId] = nullptr;

	return true;
}

bool CRoom::RemoveObject(uint32 layer, uint64 objectId)
{
	if (m_mapObject[layer].find(objectId) == m_mapObject[layer].end())
		return false;

	CGameObjectRef object = m_mapObject[layer][objectId];
	CPlayerRef player = dynamic_pointer_cast<CPlayer>(object);
	if (player)
		player->m_Room.store(std::weak_ptr<CRoom>());

	m_mapObject[layer].erase(objectId);

	return true;
}

void CRoom::Broadcast(CSendBufferRef sendBuffer, uint64 exceptId/* = 0*/)
{
	for (auto& player : m_Players)
	{
		if (player == nullptr)
			continue;
		if (player->PlayerInfo->player_id() == exceptId)
			continue;

		if (CGameSessionRef session = player->GetSession())
			session->Send(sendBuffer);
	}
}