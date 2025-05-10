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
#include "MonsterTriggerBox.h"
#include "MonsterArea.h"
#include "SavePositionBox.h"
#include "NPC.h"
#include "Item.h"
#include "Skill.h"

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
	if (m_LevelCollision)
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
	//CTriggerBoxRef box = CObjectUtil::CreateObject<CMonseterTriggerBox>();
	//box->SetTriggerBox(Vec3(8700.f, 0.f, 3840.f), Vec3(100.f, 100.f, 1200.f));
	////box->SetArea(Vec3(3810.f, 0.f, 4350.f), Vec3(2080.f, 100.f, 1100.f));
	////box->PushGatePosInfo({ Vec3(4850.f, 0.f, 3875.f), Vec3(200.f, 200.f, 850.f) });
	//AddObject((uint32)EObject_Type::TRIGGER, box);

	{
		CMonseterTriggerBoxRef box = CObjectUtil::CreateObject<CMonseterTriggerBox>();
		box->SetTriggerBox(Vec3(8700.f, 0.f, 3840.f), Vec3(100.f, 100.f, 600.f));
		box->SetArea(Vec3(7425.f, 0.f, 3850.f), Vec3(3000.f, 100.f, 2400.f));
		box->PushGateInfo(Vec3(8700.f, 450.f, 3840.f), Vec3(1000.f, 1000.f, 1.f), Vec3(-1.f, 0.f, 0.f), 90.f);
		box->PushGateInfo(Vec3(5950.f, 450.f, 3850.f), Vec3(1000.f, 1000.f, 1.f), Vec3(1.f, 0.f, 0.f), 90.f);
		AddObject((uint32)EObject_Type::TRIGGER, box);

		box = CObjectUtil::CreateObject<CMonseterTriggerBox>();
		box->SetTriggerBox(Vec3(4850, 0.f, 3875.f), Vec3(100.f, 100.f, 850.f));
		box->SetArea(Vec3(4000.f, 0.f, 3900), Vec3(2080.f, 100.f, 2400.f));
		box->PushGateInfo(Vec3(5000.f, 450.f, 3875.f), Vec3(1000.f, 1000.f, 1.f), Vec3(-1.f, 0.f, 0.f), 90.f);
		box->PushGateInfo(Vec3(2750.f, 450.f, 3875.f), Vec3(1000.f, 1000.f, 1.f), Vec3(1.f, 0.f, 0.f), 90.f);
		AddObject((uint32)EObject_Type::TRIGGER, box);

		box = CObjectUtil::CreateObject<CMonseterTriggerBox>();
		box->SetTriggerBox(Vec3(1155, 0.f, 3950.f), Vec3(30.f, 100.f, 500.f));
		box->SetArea(Vec3(0.f, 0.f, 3400), Vec3(1200.f, 100.f, 2400.f));
		box->PushGateInfo(Vec3(1200.f, 250.f, 3900.f), Vec3(500.f, 500.f, 1.f), Vec3(-1.f, 0.f, 0.f), 90.f);
		box->PushGateInfo(Vec3(-1100.f, 250.f, 4050.f), Vec3(500.f, 500.f, 1.f), Vec3(1.f, 0.f, 0.f), 90.f);
		box->PushGateInfo(Vec3(-375.f, 450.f, 2300.f), Vec3(1000.f, 1000.f, 1.f), Vec3(1.f, 0.f, 0.f), 0.f);
		AddObject((uint32)EObject_Type::TRIGGER, box);

		box = CObjectUtil::CreateObject<CMonseterTriggerBox>();
		box->SetTriggerBox(Vec3(-1350.f, 0.f, 6650.f), Vec3(500.f, 100.f, 100.f));
		box->SetArea(Vec3(-1400.f, 0.f, 7650.f), Vec3(1000.f, 100.f, 2500.f));
		box->PushGateInfo(Vec3(-1350.f, 250.f, 8600.f), Vec3(500.f, 500.f, 1.f), Vec3(-1.f, 0.f, 0.f), 0.f);
		box->PushGateInfo(Vec3(-1350.f, 250.f, 6700.f), Vec3(500.f, 500.f, 1.f), Vec3(1.f, 0.f, 0.f), 0.f);
		AddObject((uint32)EObject_Type::TRIGGER, box);
	}

	{
		CSavePositionBoxRef box = CObjectUtil::CreateObject<CSavePositionBox>();
		box->SetTriggerBox(Vec3(4850, 0.f, 3875.f), Vec3(100.f, 100.f, 850.f));
		AddObject((uint32)EObject_Type::TRIGGER, box);

		box = CObjectUtil::CreateObject<CSavePositionBox>();
		box->SetTriggerBox(Vec3(10500.f, 0.f, 3850.f), Vec3(100.f, 100.f, 700.f));
		AddObject((uint32)EObject_Type::TRIGGER, box);

		box = CObjectUtil::CreateObject<CSavePositionBox>();
		box->SetTriggerBox(Vec3(8500, 0.f, 4000.f), Vec3(100.f, 100.f, 1050.f));
		AddObject((uint32)EObject_Type::TRIGGER, box);

		box = CObjectUtil::CreateObject<CSavePositionBox>();
		box->SetTriggerBox(Vec3(1170, 0.f, 3950.f), Vec3(100.f, 100.f, 300.f));
		AddObject((uint32)EObject_Type::TRIGGER, box);

		box = CObjectUtil::CreateObject<CSavePositionBox>();
		box->SetTriggerBox(Vec3(-1325.f, 0.f, 11700.f), Vec3(300.f, 100.f, 100.f));
		AddObject((uint32)EObject_Type::TRIGGER, box);

	}
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
			if (gameObject)
			{
				gameObject->Update(m_DeltaTime);
			}
		}
	}

	for (const auto& player : m_Players)
	{
		if (player)
		{
			player->Update(m_DeltaTime);
			UpdatePlayerGravity(player, m_DeltaTime);
		}
	}

	m_LevelCollision->Collision();

	// 몬스터 정보 전송
	UpdateClients();

	RemoveLast();
	// 33ms 이후 실행 예약
	// => 대충 30프레임
	DoTimer(33, &CRoom::Update);
}

void CRoom::UpdateClients()
{
	UpdateMonster();
	UpdateAreas();
}

void CRoom::UpdateMonster()
{
	std::unordered_map<uint64, CGameObjectRef> monsters = GetLayerObjects((int)EObject_Type::Monster);

	Protocol::S_MONSTER_INFO pkt;
	for (const auto& pair : monsters)
	{
		CMonster* monster = (CMonster*)(pair.second.get());
		if (!monster->GetIsActive())
			continue;

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

		Protocol::Vector3* scale = destPosInfo->mutable_size();
		scale->set_x(srcPosInfo.size().x());
		scale->set_y(srcPosInfo.size().y());
		scale->set_z(srcPosInfo.size().z());

		destPosInfo->set_state(monster->GetState());

		const auto& stats = monster->GetAbility();
		info->mutable_monster_ablity()->set_hp(stats->currentHp);
		info->mutable_monster_ablity()->set_maxhp(stats->maxHp);
	}
	CSendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(pkt);
	Broadcast(sendBuffer, -1);
}

void CRoom::UpdateProjectile()
{
}

void CRoom::UpdateAreas()
{
	std::unordered_map<uint64, CGameObjectRef> monsters = GetLayerObjects((int)EObject_Type::Monster);
	for (int i = m_Areas.size() - 1; i >= 0; --i)
	{
		auto& monstersId = m_Areas[i]->GetMonstersId();
		monstersId.erase(std::remove_if(monstersId.begin(), monstersId.end(),
			[&](uint32 id) { return monsters.find(id) == monsters.end(); }), monstersId.end());

		if (monstersId.empty())
		{
			std::unordered_map<uint64, CGameObjectRef> walls = GetLayerObjects((int)EObject_Type::Wall);

			auto& gateIds = m_Areas[i]->GetGatesId();
			Protocol::S_GATE_CLOSE pkt;

			auto& colliders = GetLevelCollision()->GetLayerCollider((uint32)ECollision_Channel::Wall);

			for (uint32 gateid : gateIds)
			{
				CGameObjectRef object = GetLayerObject((uint32)EObject_Type::Wall, gateid);
				CBoxCollider* box = object->GetCollider();

				colliders.erase(std::remove(colliders.begin(), colliders.end(), box), colliders.end());

				Protocol::ObjectInfo* info = pkt.add_cloase_objects();
				info->set_object_id(gateid);
				RemoveObject((int)EObject_Type::Monster, gateid);
			}
			CSendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(pkt);
			Broadcast(sendBuffer, -1);

			m_Areas.erase(m_Areas.begin() + i);
		}
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
	position->set_y(0.f);
	position->set_z(1127.f);

	newPlayer->PlayerInfo->mutable_object_info()->mutable_pos_info()->set_allocated_position(position);

	switch (newPlayer->GetAttribution())
	{
	case EAttribution::FIRE:
	{
		newPlayer->PlayerInfo->set_player_type(Protocol::PLAYER_TYPE_FIRE);
	}
	break;
	case EAttribution::ICE:
	{
		newPlayer->PlayerInfo->set_player_type(Protocol::PLAYER_TYPE_ICE);
	}
	break;
	case EAttribution::LIGHTNING:
	{
		newPlayer->PlayerInfo->set_player_type(Protocol::PLAYER_TYPE_LIGHTNING);
	}
	break;
	}


	newPlayer->GetCollider()->SetCollisionProfile("Player");
	newPlayer->GetCollider()->SetBoxInfo(Vec3(11240.f, 0.f, 1127.f), Vec3(120.f, 200.f, 64.f), Vec3(0.f, 0.f, 0.f), Vec3(0.f, 100.f, 0.f));

	GetLevelCollision()->AddCollider(newPlayer->GetCollider(), ECollision_Channel::Player);

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
			playerInfo->CopyFrom(*(player->PlayerInfo));
			std::cout << playerInfo->object_info().pos_info().position().x() << " " << playerInfo->object_info().pos_info().position().y() << " " << playerInfo->object_info().pos_info().position().z() << '\n';
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

bool CRoom::HandleSpawnNPC(CPlayerRef player)
{
	std::unordered_map<uint64, CGameObjectRef> npcs = GetLayerObjects((int)EObject_Type::NPC);

	Protocol::S_SPAWN_NPC pkt;

	for (const auto& pair : npcs)
	{
		CNPC* npc = (CNPC*)(pair.second.get());
		if (!npc)
			continue;

		Protocol::NpcInfo* info = pkt.add_npc_info();

		info->set_object_id(npc->ObjectInfo->object_id());

		const Protocol::PosInfo& srcPosInfo = npc->ObjectInfo->pos_info();

		Protocol::PosInfo* destPosInfo = info->mutable_object_info()->mutable_pos_info();

		Protocol::Vector3* pos = destPosInfo->mutable_position();
		pos->set_x(srcPosInfo.position().x());
		pos->set_y(srcPosInfo.position().y());
		pos->set_z(srcPosInfo.position().z());

		Protocol::Vector3* rot = destPosInfo->mutable_rotation();
		rot->set_x(srcPosInfo.rotation().x());
		rot->set_y(srcPosInfo.rotation().y());
		rot->set_z(srcPosInfo.rotation().z());

		destPosInfo->set_state(npc->ObjectInfo->pos_info().state());

		const auto& itemList = npc->GetItemList();
		for (const auto& item : itemList)
		{
			info->add_item_id(item->GetItemInfo().id);
		}

		const auto& skillList = npc->GetSkillList();
		for (const auto& skill : skillList)
		{
			info->add_skill_id(skill->GetSkillInfo().id);
		}
	}
	CSendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(pkt);
	if (auto session = player->GetSession())
		session->Send(sendBuffer);

	return true;
}

bool CRoom::HandlePlayerInit(CPlayerRef player)
{


	return true;
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
	moveAmount.y = 0;
	moveAmount.z /= static_cast<float>(step);

	const auto& dir = player->GetDir();

	for (int i = 1; i <= step; ++i)
	{
		nowPos.x += moveAmount.x;
		nowPos.y += 0;
		nowPos.z += moveAmount.z;

		ToProtoVector3(&protoNow, nowPos);

		const auto& box = player->GetCollider();
		box->Update();

		if (m_LevelCollision->CollisionWithWall(box) || m_LevelCollision->CollisionWithOnlyChannel(ECollision_Channel::Player, box)
			|| m_LevelCollision->CollisionWithOnlyChannel(ECollision_Channel::NPC, box))
		{
			nowPos.x -= moveAmount.x;
			nowPos.z -= moveAmount.z;

			XMVECTOR dirVec = XMVectorSet(dir.x(), dir.y(), dir.z(), 0.0f);
			dirVec = XMVector3Normalize(dirVec);
			if (player->GetState() != Protocol::MOVE_STATE_DASH && player->GetState() != Protocol::MOVE_STATE_DASH_END &&
				player->GetState() != Protocol::MOVE_STATE_DAMAGED && player->GetState() != Protocol::MOVE_STATE_DAMAGED_END)
			{
				dirVec = XMVectorScale(dirVec, -20);
			}

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

	CSendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(movePkt);
	Broadcast(sendBuffer, player->PlayerInfo->player_id());

	if (auto session = player->GetSession())
		session->Send(sendBuffer);

	player->GetCollider()->SetGateDir(Vec3(dir.x(), dir.y(), dir.z()));
	return true;
}

bool CRoom::HandleActPlayer(CPlayerRef player)
{
	const auto& dir = player->GetDir();

	Protocol::S_ACT movePkt;
	auto* moveInfo = movePkt.mutable_player_move_info();
	moveInfo->set_player_id(player->PlayerInfo->player_id());

	auto* posInfo = moveInfo->mutable_pos_info();
	const auto& pos = player->PlayerInfo->object_info().pos_info().position();
	ToProtoVector3(posInfo->mutable_position(), XMFLOAT3(pos.x(), pos.y(), pos.z()));

	const auto& rot = player->PlayerInfo->object_info().pos_info().rotation();
	ToProtoVector3(posInfo->mutable_rotation(), XMFLOAT3(rot.x(), rot.y(), rot.z()));

	posInfo->set_state(player->GetState());

	CSendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(movePkt);
	Broadcast(sendBuffer, player->PlayerInfo->player_id());

	if (auto session = player->GetSession())
		session->Send(sendBuffer);

	player->GetCollider()->SetGateDir(Vec3(dir.x(), dir.y(), dir.z()));
	return true;
}

bool CRoom::UpdatePlayerGravity(CPlayerRef player, float deltaTime)
{
	if (player->GetState() == Protocol::MOVE_STATE_DASH || player->GetState() == Protocol::MOVE_STATE_FALLING_END)
		return true;


	if (player->GetState() == Protocol::MOVE_STATE_FALLING)
	{
		XMFLOAT3 moveAmount = XMFLOAT3(0.f, deltaTime * GRAVITY * 10, 0.f);
		const auto& now = player->PlayerInfo->mutable_object_info()->mutable_pos_info()->mutable_position();
		now->set_y(now->y() - moveAmount.y);

		if (now->y() <= -1000.f)
		{
			player->GetAbility()->currentHp -= 20.f;
			if (player->GetAbility()->currentHp <= 0.f)
			{
				player->SetState(Protocol::MOVE_STATE_DEATH);
			}
			else
			{
				player->SetState(Protocol::MOVE_STATE_FALLING_END);
			}
			Vec3 safePos = player->GetSafePosition();
			now->set_x(safePos.x);
			now->set_y(safePos.y);
			now->set_z(safePos.z);
			UpdatePlayerAbility(player);
		}



		Protocol::S_MOVE movePkt;
		auto* moveInfo = movePkt.mutable_player_move_info();
		moveInfo->set_player_id(player->PlayerInfo->player_id());
		auto* posInfo = moveInfo->mutable_pos_info();
		posInfo->mutable_position()->set_x(now->x());
		posInfo->mutable_position()->set_y(now->y());
		posInfo->mutable_position()->set_z(now->z());

		const auto& rot = player->PlayerInfo->object_info().pos_info().rotation();
		ToProtoVector3(posInfo->mutable_rotation(), XMFLOAT3(rot.x(), rot.y(), rot.z()));


		posInfo->set_state(player->GetState());

		CSendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(movePkt);
		if (auto session = player->GetSession())
			session->Send(sendBuffer);
	}
	else
	{
		auto box = player->GetCollider();
		box->SetBoxHeight(0.f);
		if (!m_LevelCollision->CollisionWithWall(box))
		{
			player->SetState(Protocol::MOVE_STATE_FALLING);
			UpdatePlayerState(player);
		}
	}
	return true;
}

bool CRoom::UpdatePlayerAbility(CPlayerRef player)
{
	Protocol::S_UPDATE_PLAYER_STATS pkt;
	const auto& ablity = player->GetAbility();
	pkt.set_player_id(player->PlayerInfo->player_id());
	pkt.mutable_player_ability()->set_damage(ablity->attack);
	pkt.mutable_player_ability()->set_hp(ablity->currentHp);
	pkt.mutable_player_ability()->set_maxhp(ablity->maxHp);
	pkt.mutable_player_ability()->set_gold(ablity->gold);
	CSendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(pkt);
	Broadcast(sendBuffer, -1);
	//std::cout << ablity->currentHp << '\n';
	return true;
}

bool CRoom::UpdatePlayerState(CPlayerRef player)
{
	Protocol::S_UPDATE_PLAYER_STATE pkt;

	pkt.set_player_id(player->PlayerInfo->player_id());
	pkt.set_state(player->GetState());

	CSendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(pkt);
	Broadcast(sendBuffer, -1);
	return true;
}

bool CRoom::HandleSpawnProjectile(CProjectileRef projectile)
{
	AddProjectile(projectile);

	Protocol::S_SPAWN_PROJECTILE_SUCESSE pkt;
	pkt.set_projectile_id(projectile->ProjectileInfo->projectile_id());
	pkt.mutable_size()->set_x(projectile->GetStateInfo().Size.x);
	pkt.mutable_size()->set_y(projectile->GetStateInfo().Size.y);
	pkt.mutable_size()->set_z(projectile->GetStateInfo().Size.z);
	pkt.set_mesh(projectile->m_meshType);

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
		g_pool->Release(projectile);
	}
	else
	{
		int step = 1;

		Protocol::Vector3& protoNow = *projectile->ProjectileInfo->mutable_object_info()->mutable_pos_info()->mutable_position();
		XMFLOAT3 nowPos(protoNow.x(), protoNow.y(), protoNow.z());
		// 이동량
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

			if (projectile->GetCollisionExplosion())
			{
				if (m_LevelCollision->CollisionWithWall(projectile->GetCollider()))
				{
					projectile->ProjectileInfo->set_state(Protocol::COLLISION);
					m_mapObject[(uint32)EObject_Type::Projectile].erase(projectile->ProjectileInfo->projectile_id());
					g_pool->Release(projectile);
					break;
				}
			}
		}

		const auto& rot = projectile->ProjectileInfo->mutable_object_info()->mutable_pos_info()->rotation();
		const auto& scale = projectile->ProjectileInfo->mutable_object_info()->mutable_pos_info()->size();
		auto* info = pkt.mutable_projectile_info();
		info->set_projectile_id(projectile->ProjectileInfo->projectile_id());
		info->set_state(projectile->ProjectileInfo->state());

		auto* posInfo = pkt.mutable_projectile_info()->mutable_object_info()->mutable_pos_info();
		posInfo->mutable_position()->set_x(nowPos.x);
		posInfo->mutable_position()->set_y(nowPos.y);
		posInfo->mutable_position()->set_z(nowPos.z);

		posInfo->mutable_size()->set_x(scale.x());
		posInfo->mutable_size()->set_y(scale.y());
		posInfo->mutable_size()->set_z(scale.z());

		posInfo->mutable_rotation()->set_x(rot.x());
		posInfo->mutable_rotation()->set_y(rot.y());
		posInfo->mutable_rotation()->set_z(rot.z());
	}
	CSendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(pkt);
	Broadcast(sendBuffer, -1);
	//if (auto session = player->GetSession())
	//	session->Send(sendBuffer);

	return true;
}

bool CRoom::HandleMoveProjectileEffect(CProjectileRef projectile)
{
	Protocol::S_PROJECTILE_EFFECT pkt;
	pkt.set_projectile_id(projectile->ProjectileInfo->projectile_id());
	CSendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(pkt);
	Broadcast(sendBuffer, -1);
	return true;
}

bool CRoom::HandleBuyItem(CPlayerRef player, CItemRef item)
{
	std::unordered_map<uint64, CGameObjectRef> npcs = GetLayerObjects((int)EObject_Type::NPC);
	// npc가 한명이여서 지금은 이렇게 진행, 여러명일 경우 패킷으로 역할 구분해야
	for (const auto& pair : npcs)
	{
		CNPC* npc = (CNPC*)(pair.second.get());
		if (!npc)
			continue;

		auto& itemList = npc->GetItemList();

		auto iter = find_if(itemList.begin(), itemList.end(), [&](CItemRef tem) {
			return tem->GetItemInfo().id == item->GetItemInfo().id;
			});

		if (iter != itemList.end())
		{
			if (player->BuyItem(item))
			{
				// 성공메시지 전달
				IsBuyItem(player, item, true);
				(*iter)->GetItemInfo().bSell = true;
			}
			else
			{
				// 실패메시지 전달
				std::cout << "실패\n";
				IsBuyItem(player, nullptr, false);
			}
		}
		UpdateItem(npc->ObjectInfo->object_id());
	}


	return true;
}

bool CRoom::HandleBuySkill(CPlayerRef player, CSkillRef skill)
{
	std::unordered_map<uint64, CGameObjectRef> npcs = GetLayerObjects((int)EObject_Type::NPC);
	// npc가 한명이여서 지금은 이렇게 진행, 여러명일 경우 패킷으로 역할 구분해야
	for (const auto& pair : npcs)
	{
		CNPC* npc = (CNPC*)(pair.second.get());
		if (!npc)
			continue;

		auto& skillList = npc->GetSkillList();

		auto iter = find_if(skillList.begin(), skillList.end(), [&](CSkillRef s) {
			return s->GetSkillInfo().id == skill->GetSkillInfo().id;
			});

		if (iter != skillList.end())
		{
			if (player->BuySkill(skill))
			{
				// 성공메시지 전달
				IsBuySkill(player, skill, true);
				(*iter)->GetSkillInfo().bSell = true;
			}
			else
			{
				// 실패메시지 전달
				IsBuyItem(player, nullptr, false);
			}
		}
		UpdateItem(npc->ObjectInfo->object_id());
	}


	return true;
}

bool CRoom::IsBuySkill(CPlayerRef player, CSkillRef skill, bool isBuy)
{
	Protocol::S_BUY_SKILL pkt;
	pkt.set_player_id(player->PlayerInfo->player_id());
	pkt.set_is_success(isBuy);
	if (skill)
	{
		pkt.set_skill_id(skill->GetSkillInfo().id);
	}
	CSendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(pkt);
	Broadcast(sendBuffer, -1);
	return true;
}

bool CRoom::IsBuyItem(CPlayerRef player, CItemRef item, bool isBuy)
{
	Protocol::S_BUY_ITEM pkt;
	pkt.set_player_id(player->PlayerInfo->player_id());
	pkt.set_is_success(isBuy);
	if (item)
	{
		pkt.set_item_id(item->GetItemInfo().id);
	}
	CSendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(pkt);
	if (auto session = player->GetSession())
		session->Send(sendBuffer);
	return true;
}

bool CRoom::UpdateItem(uint32 npcId)
{
	std::unordered_map<uint64, CGameObjectRef> npcs = GetLayerObjects((int)EObject_Type::NPC);
	Protocol::S_UPDATE_ITEM pkt;
	pkt.set_npc_id(npcId);
	for (const auto& pair : npcs)
	{
		CNPC* npc = (CNPC*)(pair.second.get());
		if (!npc)
			continue;

		const auto& itemList = npc->GetItemList();
		for (const auto& item : itemList)
		{
			Protocol::ItemInfo* info = pkt.add_item_info();
			info->set_is_sell(item->GetItemInfo().bSell);
			info->set_item_id(item->GetItemInfo().id);
		}

		const auto& skillList = npc->GetSkillList();
		for (const auto& skill : skillList)
		{
			Protocol::SkillInfo* info = pkt.add_skill_info();
			info->set_is_sell(skill->GetSkillInfo().bSell);
			info->set_skill_id(skill->GetSkillInfo().id);
		}
	}
	CSendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(pkt);
	Broadcast(sendBuffer, -1);

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

bool CRoom::AddMonster(CMonsterRef object)
{
	if (m_mapObject[(uint32)EObject_Type::Monster].find(object->MonsterInfo->object_id()) != m_mapObject[(uint32)EObject_Type::Monster].end())
		return false;

	m_mapObject[(uint32)EObject_Type::Monster].insert(make_pair(object->MonsterInfo->object_id(), object));

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

bool CRoom::RemoveLast()
{
	for (auto& key : m_deleteObjects)
	{
		if (m_mapObject[key.first].find(key.second) == m_mapObject[key.first].end())
			continue;

		m_mapObject[key.first].erase(key.second);
	}
	m_deleteObjects.clear();
	return true;
}

bool CRoom::RemoveObject(uint32 layer, uint64 id)
{
	m_deleteObjects.push_back({ layer, id });

	return true;
}

bool CRoom::HandleOpenGate(const std::vector<GateInfo>& posInfo)
{
	Protocol::S_GATE_OPNE pkt;

	for (int i = 0; i < posInfo.size(); ++i)
	{
		CGameObjectRef object = CObjectUtil::CreateObject<CGameObject>();
		const auto& objectPosInfo = object->ObjectInfo->mutable_pos_info();

		objectPosInfo->mutable_position()->set_x(posInfo[i].GatePos.x);
		objectPosInfo->mutable_position()->set_y(posInfo[i].GatePos.y);
		objectPosInfo->mutable_position()->set_z(posInfo[i].GatePos.z);

		objectPosInfo->mutable_rotation()->set_y(posInfo[i].GateYRot);

		objectPosInfo->mutable_size()->set_x(posInfo[i].GateSize.x);
		objectPosInfo->mutable_size()->set_y(posInfo[i].GateSize.y);
		objectPosInfo->mutable_size()->set_z(posInfo[i].GateSize.z);

		object->GetCollider()->SetCollisionProfile("Wall");
		object->GetCollider()->SetBoxInfo(posInfo[i].GatePos, posInfo[i].GateSize, Vec3(0.f, posInfo[i].GateYRot, 0.f));
		object->GetCollider()->SetGateDir(posInfo[i].GateDir);
		GetLevelCollision()->AddCollider(object->GetCollider(), ECollision_Channel::Wall);
		AddObject((uint32)EObject_Type::Wall, object);

		Protocol::ObjectInfo* info = pkt.add_open_objects();
		info->CopyFrom(*object->ObjectInfo);

		m_Areas[m_Areas.size() - 1]->PushGateId(object->ObjectInfo->object_id());
	}
	CSendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(pkt);
	Broadcast(sendBuffer, -1);

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