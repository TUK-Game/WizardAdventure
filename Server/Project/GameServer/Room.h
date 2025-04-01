#pragma once

#include "JobQueue.h"

class CRoom : public CJobQueue
{
public:
	CRoom();
	virtual ~CRoom();

public:
	CRoomRef GetRoomRef();

public:
	void Update();

public:
	bool HandleEnterPlayer(CPlayerRef player);
	bool HandleLeavePlayer(CPlayerRef player);

private:
	bool AddPlayer(CPlayerRef player);
	bool RemovePlayer(uint64 playerId);
	bool AddObject(CGameObjectRef object);
	bool RemoveObject(uint64 objectId);

	bool EnterRoom(CPlayerRef object, bool bRandPos = true);
	bool LeaveRoom(CPlayerRef object);

private:
	void Broadcast(CSendBufferRef sendBuffer, uint64 exceptId = 0);

private:
	std::unordered_map<uint64, CGameObjectRef> m_mapObject;
	// TODO: player array
	std::array<CPlayerRef, MAX_PLAYERS> m_Players;
};

// TEMP
// TODO: RoomManager
extern CRoomRef g_Room;	