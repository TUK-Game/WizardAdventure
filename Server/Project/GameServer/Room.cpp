#include "pch.h"
#include "Room.h"
#include "Player.h"
#include "GameSession.h"
#include "Monster.h"
#include "ObjectUtil.h"

CRoomRef g_Room = std::make_shared<CRoom>();

CRoom::CRoom()
{
}

CRoom::~CRoom()
{
}

CRoomRef CRoom::GetRoomRef()
{
	return static_pointer_cast<CRoom>(shared_from_this());
}

void CRoom::Update()
{
	//std::cout << "Update Room" << std::endl;

	// 33ms 이후 실행 예약
	// => 대충 30프레임
	DoTimer(33, &CRoom::Update);
}

bool CRoom::EnterRoom(CGameObjectRef object, bool bRandPos /*= true*/)
{
	bool success = AddObject(object);

	if (success)
	{
		std::cout << "Enter Player " << object->ObjectInfo->object_id() << std::endl;
	}

	// 랜덤 위치
	if (bRandPos)
	{
		object->PosInfo->set_x(CUtil::GetRandom(0.f, 500.f));
		object->PosInfo->set_y(CUtil::GetRandom(0.f, 500.f));
		object->PosInfo->set_z(100.f);
		object->PosInfo->set_yaw(CUtil::GetRandom(0.f, 100.f));
	}

	// 입장 사실을 새 플레이어에게 알린다
	if (auto player = dynamic_pointer_cast<CPlayer>(object))
	{
		Protocol::S_ENTER_GAME enterGamePkt;
		enterGamePkt.set_success(success);

		Protocol::ObjectInfo* playerInfo = new Protocol::ObjectInfo();
		playerInfo->CopyFrom(*object->ObjectInfo);
		enterGamePkt.set_allocated_player(playerInfo);

		CSendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(enterGamePkt);
		if (auto session = player->GetSession())
			session->Send(sendBuffer);
	}

	// 입장 사실을 다른 플레이어에게 알린다
	{
		Protocol::S_SPAWN spawnPkt;

		Protocol::ObjectInfo* objectInfo = spawnPkt.add_players();
		objectInfo->CopyFrom(*object->ObjectInfo);

		CSendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(spawnPkt);
		Broadcast(sendBuffer, object->ObjectInfo->object_id());
	}

	// 기존의 플레이어 정보를 새 플레이어에게 알린다
	if (auto player = dynamic_pointer_cast<CPlayer>(object))
	{
		Protocol::S_SPAWN spawnPkt;

		for (auto& item : m_mapObject)
		{
			if (item.second->IsPlayer() == false)
				continue;

			Protocol::ObjectInfo* playerInfo = spawnPkt.add_players();
			playerInfo->CopyFrom(*item.second->ObjectInfo);
		}

		CSendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(spawnPkt);
		if (auto session = player->GetSession())
			session->Send(sendBuffer);
	}

	return success;
}

bool CRoom::LeaveRoom(CGameObjectRef object)
{
	if (object == nullptr)
		return false;

	const uint64 objectId = object->ObjectInfo->object_id();
	bool success = RemoveObject(objectId);

	if (success)
	{
		std::cout << "Leave Player " << object->ObjectInfo->object_id() << std::endl;
	}

	// 퇴장 사실을 퇴장하는 플레이어에게 알린다
	if (auto player = dynamic_pointer_cast<CPlayer>(object))
	{
		Protocol::S_LEAVE_GAME leaveGamePkt;

		CSendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(leaveGamePkt);
		if (auto session = player->GetSession())
			session->Send(sendBuffer);
	}

	// 퇴장 사실을 알린다
	{
		Protocol::S_DESPAWN despawnPkt;
		despawnPkt.add_object_ids(objectId);

		CSendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(despawnPkt);
		Broadcast(sendBuffer, objectId);

		if (auto player = dynamic_pointer_cast<CPlayer>(object))
			if (auto session = player->GetSession())
				session->Send(sendBuffer);
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

bool CRoom::AddObject(CGameObjectRef object)
{
	if (m_mapObject.find(object->ObjectInfo->object_id()) != m_mapObject.end())
		return false;

	m_mapObject.insert(make_pair(object->ObjectInfo->object_id(), object));

	object->m_Room.store(GetRoomRef());

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
	// TEMP
	// TODO: mapObject(X) players(O)
	for (auto& item : m_mapObject)
	{
		CPlayerRef player = dynamic_pointer_cast<CPlayer>(item.second);
		if (player == nullptr)
			continue;
		if (player->ObjectInfo->object_id() == exceptId)
			continue;

		if (CGameSessionRef session = player->GetSession())
			session->Send(sendBuffer);
	}
}