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
	void Init();
	void Update();
	void UpdateClients();
public:
	class CLevelCollision* GetLevelCollision() { return m_LevelCollision; }

public:
	std::unordered_map<uint64, CGameObjectRef>& GetLayerObjects(uint32 layer) { return m_mapObject[layer]; }

public:
	bool HandleEnterPlayer(CPlayerRef player);
	bool HandleLeavePlayer(CPlayerRef player);

	bool HandleMovePlayer(CPlayerRef player);

	bool AddObject(uint32 layer, CGameObjectRef object);
private:
	bool AddPlayer(CPlayerRef player);
	bool RemovePlayer(uint64 playerId);
	bool RemoveObject(uint32 layer, uint64 objectId);

	bool EnterRoom(CPlayerRef object, bool bRandPos = true);
	bool LeaveRoom(CPlayerRef object);

private:
	void Broadcast(CSendBufferRef sendBuffer, uint64 exceptId = 0);

private:
	std::array<std::unordered_map<uint64, CGameObjectRef>, (int)EObject_Type::Max> m_mapObject;
	std::array<CPlayerRef, MAX_PLAYERS> m_Players;

	class CLevelCollision* m_LevelCollision;
	float m_DeltaTime = 0.f;
};

// TEMP
// TODO: RoomManager
extern CRoomRef g_Room;

void ToProtoVector3(Protocol::Vector3* to, const XMFLOAT3& from);
