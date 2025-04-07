#pragma once

#include "JobQueue.h"
#include "DirectXCollision.h"

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
	class CLevelCollision* GetLevelCollision() { return m_LevelCollision; }

public:
	bool HandleEnterPlayer(CPlayerRef player);
	bool HandleLeavePlayer(CPlayerRef player);

	bool HandleMovePlayer(CPlayerRef player);

	bool AddObject(CGameObjectRef object);
private:
	bool AddPlayer(CPlayerRef player);
	bool RemovePlayer(uint64 playerId);
	bool RemoveObject(uint64 objectId);

	bool EnterRoom(CPlayerRef object, bool bRandPos = true);
	bool LeaveRoom(CPlayerRef object);

private:
	void Broadcast(CSendBufferRef sendBuffer, uint64 exceptId = 0);

private:
	std::unordered_map<uint64, CGameObjectRef> m_mapObject;
	std::array<CPlayerRef, MAX_PLAYERS> m_Players;

	class CLevelCollision* m_LevelCollision;
	float m_DeltaTime = 0.f;
};

// TEMP
// TODO: RoomManager
extern CRoomRef g_Room;	