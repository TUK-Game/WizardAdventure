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
	bool AddObject(CGameObjectRef object);
	bool RemoveObject(uint64 objectId);

	bool EnterRoom(CGameObjectRef object, bool bRandPos = true);
	bool LeaveRoom(CGameObjectRef object);

private:
	void Broadcast(CSendBufferRef sendBuffer, uint64 exceptId = 0);

private:
	std::unordered_map<uint64, CGameObjectRef> m_mapObject;
	// TODO: player array
};

// TEMP
// TODO: RoomManager
extern CRoomRef g_Room;	