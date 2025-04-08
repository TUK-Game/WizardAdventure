#include "pch.h"
#include "Room.h"
#include "Player.h"
#include "GameSession.h"
#include "Monster.h"
#include "ObjectUtil.h"
#include "LevelCollision.h"
#include "BoxCollider.h"

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

void CRoom::Update()
{
	//std::cout << "Update Room" << std::endl;
	g_Timer->Update();
	m_DeltaTime = g_Timer->GetDeltaTime();

	for (const auto& object : m_mapObject)
	{
		auto& gameObject = object.second;

		gameObject->Update();	
	}

	for (const auto& player : m_Players)
	{
		if(player)
			player->Update();
	}

	//m_LevelCollision->Collision();

	// 33ms 이후 실행 예약
	// => 대충 30프레임
	DoTimer(33, &CRoom::Update);
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
	newPlayer->GetCollider()->SetBoxInfo(XMFLOAT3(11240.f, 20.f, 1127.f), XMFLOAT3(100.f, 200.f, 24.f), XMFLOAT3(0.f, 100.f, 0.f));

	// 입장 사실을 새 플레이어에게 알린다
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

	// 입장 사실을 다른 플레이어에게 알린다
	{
		Protocol::S_SPAWN_NEW_PLAYER spawnPkt;

		Protocol::PlayerInfo* playerInfo = spawnPkt.mutable_player();
		playerInfo->CopyFrom(*(newPlayer->PlayerInfo));

		CSendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(spawnPkt);
		Broadcast(sendBuffer, newPlayer->PlayerInfo->player_id());
	}

	// 기존의 플레이어 정보를 새 플레이어에게 알린다
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

	// 퇴장 사실을 퇴장하는 플레이어에게 알린다
	{
		Protocol::S_LEAVE_GAME leaveGamePkt;

		CSendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(leaveGamePkt);
		if (auto session = leavePlayer->GetSession())
			session->Send(sendBuffer);
	}

	// 퇴장 사실을 알린다
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
	// 1. 충돌체크
	// 2. 위치조정
	// 3. 모든 클라에 위치 전송
	int step = 1;

	Protocol::Vector3& protoNow = *player->PlayerInfo->mutable_object_info()->mutable_pos_info()->mutable_position();
	XMFLOAT3 nowPos(protoNow.x(), protoNow.y(), protoNow.z());

	// 이동량
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
			std::cout << "박음" << std::endl;

			nowPos.x -= moveAmount.x;
			nowPos.y -= moveAmount.y;
			nowPos.z -= moveAmount.z;

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

	std::cout << "보냄 " << nowPos.x << " " << nowPos.y << " " << nowPos.z << '\n';

	CSendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(movePkt);
	Broadcast(sendBuffer, player->PlayerInfo->player_id());

	if (auto session = player->GetSession())
		session->Send(sendBuffer);

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

bool CRoom::AddObject(CGameObjectRef object)
{
	if (m_mapObject.find(object->ObjectInfo->object_id()) != m_mapObject.end())
		return false;

	m_mapObject.insert(make_pair(object->ObjectInfo->object_id(), object));

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

bool CRoom::RemoveObject(uint64 objectId)
{
	if (m_mapObject.find(objectId) == m_mapObject.end())
		return false;

	CGameObjectRef object = m_mapObject[objectId];
	CPlayerRef player = dynamic_pointer_cast<CPlayer>(object);
	if (player)
		player->m_Room.store(std::weak_ptr<CRoom>());

	m_mapObject.erase(objectId);

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