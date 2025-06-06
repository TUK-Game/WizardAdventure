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

	void UpdateMonster();
	void UpdateProjectile();
	void UpdateAreas();
public:
	class CLevelCollision* GetLevelCollision() { return m_LevelCollision; }
	std::unordered_map<uint64, CGameObjectRef>& GetLayerObjects(uint32 layer) { return m_mapObject[layer]; }
	CGameObjectRef GetLayerObject(uint32 layer, uint64 id) { return m_mapObject[layer][id]; }
	std::array<CPlayerRef, MAX_PLAYERS>& GetPlayers() { return m_Players; }
	std::vector<class CMonsterArea*>& GetAreas() { return m_Areas; }

public:
	bool HandleEnterPlayer(CPlayerRef player);
	bool HandleSpawnNPC(CPlayerRef player);
	bool HandlePlayerInit(CPlayerRef player);
	bool HandleLeavePlayer(CPlayerRef player);

	bool HandleMovePlayer(CPlayerRef player);
	bool HandleActPlayer(CPlayerRef player);
	bool UpdatePlayerGravity(CPlayerRef player, float deltaTime);
	bool UpdatePlayerAbility(CPlayerRef player);
	bool UpdatePlayerState(CPlayerRef player);

	bool HandleSpawnProjectile(CProjectileRef projectile);
	bool HandleMoveProjectile(CProjectileRef projectile);
	bool HandleMoveProjectileEffect(CProjectileRef projectile);

	bool HandleBuyItem(CPlayerRef player, CItemRef item);
	bool IsBuyItem(CPlayerRef player, CItemRef item, bool isBuy);
	bool UpdateItem(uint32 npcId);

	bool HandleBuySkill(CPlayerRef player, CSkillRef skill);
	bool IsBuySkill(CPlayerRef player, CSkillRef skill, bool isBuy);

	bool AddObject(uint32 layer, CGameObjectRef object);
	bool AddMonster(CMonsterRef object);
	bool AddProjectile(CProjectileRef object);

	bool RemoveObject(uint32 layer, uint64 id);

	bool HandleOpenGate(const std::vector<struct GateInfo>& posInfo);

private:
	bool AddPlayer(CPlayerRef player);
	bool RemovePlayer(uint64 playerId);
	bool RemoveLast();

	bool EnterRoom(CPlayerRef object, bool bRandPos = true);
	bool LeaveRoom(CPlayerRef object);

	Protocol::MonsterType ConvertMonsterTypeToProtocol(EMonsterType type);

private:
	void Broadcast(CSendBufferRef sendBuffer, uint64 exceptId = 0);

private:
	std::array<std::unordered_map<uint64, CGameObjectRef>, (int)EObject_Type::Max> m_mapObject;
	std::vector<std::pair<uint32, uint64>> m_deleteObjects;
	std::array<CPlayerRef, MAX_PLAYERS> m_Players;
	std::vector<CMonsterArea*> m_Areas;

	class CLevelCollision* m_LevelCollision;
	float m_DeltaTime = 0.f;
};

// TEMP
// TODO: RoomManager
extern CRoomRef g_Room;

void ToProtoVector3(Protocol::Vector3* to, const XMFLOAT3& from);
