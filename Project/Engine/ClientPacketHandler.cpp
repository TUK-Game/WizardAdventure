#include "pch.h"
#include "ClientPacketHandler.h"
#include "Protocol.pb.h"
#include "LevelManager.h"
#include "Player.h"
#include "Level.h"
#include "RenderManager.h"
#include "Camera.h"
#include "NetworkManager.h"
#include "ServerSession.h"
#include "Transform.h"
#include "CameraScript.h"
#include "Monster.h"
#include "Layer.h"
#include "SkillObject.h"
#include "FireBall.h"
#include "FireSword.h"
#include "FirePillar.h"
#include "FireCircle.h"
#include "ParticleSystemManager.h"
#include "MeshRenderer.h"
#include "AssetManager.h"
#include "EffectManager.h"

#include "TestWidget.h"
#include "MapPlayerWidget.h"
#include "PlayWidgetWindow.h"

PacketHandlerFunc g_PacketHandler[UINT16_MAX];

bool Handle_INVALID(CPacketSessionRef& session, BYTE* buffer, int32 len)
{
	return true;
}

bool Handle_S_LOGIN(CPacketSessionRef& session, Protocol::S_LOGIN& pkt)
{
	if (pkt.success())
	{
		std::cout << "로그인 성공!" << std::endl;
		// 입장
#ifndef AUTO_SERVER_CONNECT
		Protocol::C_ENTER_GAME enterPkt;
		
		auto sendBuffer = ClientPacketHandler::MakeSendBuffer(enterPkt);
		session->Send(sendBuffer);
#endif
		return true;
	}

	std::cout << "로그인 실패!" << std::endl;
	exit(0);

	return false;
}

bool Handle_S_ENTER_GAME(CPacketSessionRef& session, Protocol::S_ENTER_GAME& pkt)
{
	std::cout << "등장" << std::endl;
	// 플레이어 생성
	UINT64 id = pkt.player().player_id();

	CPlayer* player = new CPlayer(EPlayerAttribute::Fire, true);

	CLevel* level = CLevelManager::GetInst()->GetCurrentLevel();

	const Protocol::Vector3& position = pkt.player().object_info().pos_info().position();
	player->GetTransform()->SetRelativePosition(position.x(), position.y(), position.z());

	level->AddGameObject(player, 3, false);
	CLevelManager::GetInst()->SetOwnPlayer(player);
	CLevelManager::GetInst()->SetPlayer(player, id);
	CRenderManager::GetInst()->GetMainCamera()->SetTarget(player);
	CRenderManager::GetInst()->GetMainCamera()->SetCameraType(ECamera_Type::Fixed);

		
	CNetworkManager::GetInst()->s_GameSession->SetOwnPlayer(player);
	CNetworkManager::GetInst()->s_GameSession->SetClientID(id);

	const auto& mapwindow = level->CreateWidgetWindow<TestWidget>(EWIDGETWINDOW_TYPE::MAP_WINDOW, L"MapWindow", player);
	CPlayWidgetWindow* gamewindow = level->CreateWidgetWindow<CPlayWidgetWindow>(EWIDGETWINDOW_TYPE::GAME_WINDOW, L"GamePlayWidget", player);
	if (mapwindow)
	{
		mapwindow->SetOwnerPlayer(player);
		mapwindow->AddPlayer(player, id);
		mapwindow->SetEnable(false);
	}
	if (gamewindow)
	{
		gamewindow->SetOwnerPlayer(player);
	}

	Protocol::C_ENTER_GAME_SUCCESS GSpkt;
	switch (player->GetAttribute())
	{
	case EPlayerAttribute::Fire:
	{
		GSpkt.mutable_player()->set_player_type(Protocol::PLAYER_TYPE_FIRE);
		player->InitStats(100, 100, 30, 300.f);
		// 임시적으로 넣어둠 원래는 게임 진행하면서 스킬 획득
	/*	gamewindow->SetSkill(1, L"FireShot", 10);
		gamewindow->SetSkill(2, L"FireRain", 10);
		gamewindow->SetSkill(3, L"FireballExplosion", 10);
		gamewindow->SetSkill(4, L"FireTower", 10);
		gamewindow->SetSkill(5, L"Fireball", 10);*/
		gamewindow->SetGauge(L"HPBar", 100, true);
		gamewindow->SetGauge(L"SignautreGage", 0, false);
	}
	break;
	case EPlayerAttribute::Water:
	{
		GSpkt.mutable_player()->set_player_type(Protocol::PLAYER_TYPE_ICE);
		player->InitStats(100, 100, 30, 300.f);
	}
	break;
	case EPlayerAttribute::Electric:
	{
		GSpkt.mutable_player()->set_player_type(Protocol::PLAYER_TYPE_LIGHTNING);
		player->InitStats(100, 100, 30, 300.f);
	}
	break;
	}

	auto sendBuffer = ClientPacketHandler::MakeSendBuffer(GSpkt);
	session->Send(sendBuffer);
	return true;
}

bool Handle_S_SPAWN_PROJECTILE_SUCESSE(CPacketSessionRef& session, Protocol::S_SPAWN_PROJECTILE_SUCESSE& pkt)
{
	UINT64 id = pkt.projectile_id();
	auto& map = CLevelManager::GetInst()->GetCurrentLevel()->GetLayer(12)->GetProjectileMap();
	if (map.find(id) != map.end())
	{
		map[id]->SetEnable(true);
	}
	else
	{
		const auto& size = pkt.mutable_size();
		switch (pkt.mesh())
		{
		case Protocol::FIRE_CIRCLE:
		{
			CFireCircle* magic = new CFireCircle();
			magic->GetTransform()->SetRelativeScale(Vec3(size->x(), size->y(), size->z()));
			map[id] = magic;
			CLevelManager::GetInst()->GetCurrentLevel()->SafeAddGameObject(magic, 12, false);
		}
		break;
		case Protocol::FIRE_BALL:
		{
			CFireBall* magic = new CFireBall();
			magic->GetTransform()->SetRelativeScale(Vec3(size->x(), size->y(), size->z()));
			map[id] = magic;
			CLevelManager::GetInst()->GetCurrentLevel()->SafeAddGameObject(magic, 12, false);
		}
		break;
		case Protocol::FIRE_PILLAR:
		{
			CFirePillar* magic = new CFirePillar();
			magic->GetTransform()->SetRelativeScale(Vec3(size->x(), size->y(), size->z()));
			map[id] = magic;
			CLevelManager::GetInst()->GetCurrentLevel()->SafeAddGameObject(magic, 12, false);
		}
		break;
		case Protocol::FIRE_SWORD:
		{
			CFireSword* magic = new CFireSword();
			magic->GetTransform()->SetRelativeScale(Vec3(size->x(), size->y(), size->z()));
			map[id] = magic;
			CLevelManager::GetInst()->GetCurrentLevel()->SafeAddGameObject(magic, 12, false);
		}
		break;
		}
		
	}
	return true;
}

bool Handle_S_SPAWN_NEW_PLAYER(CPacketSessionRef& session, Protocol::S_SPAWN_NEW_PLAYER& pkt)
{
	// 1. 입장한 플레이어 받기
	const Protocol::PlayerInfo& info = pkt.player();

	CPlayer* player = new CPlayer(EPlayerAttribute::Fire);

	const Protocol::Vector3& position = pkt.player().object_info().pos_info().position();
	player->SetName(L"Player" + std::to_wstring(info.player_id()));
	player->GetTransform()->SetRelativePosition(position.x(), position.y(), position.z());
	CLevelManager::GetInst()->GetCurrentLevel()->AddGameObject(player, 3, false);
	CLevelManager::GetInst()->SetPlayer(player, info.player_id());

	const auto& window = CLevelManager::GetInst()->GetCurrentLevel()->FindWidgetWindow(EWIDGETWINDOW_TYPE::MAP_WINDOW);
	if (window)
	{
		((TestWidget*)window)->AddPlayer(player, info.player_id());
	}
	return true;
}

bool Handle_S_SPAWN_EXISTING_PLAYER(CPacketSessionRef& session, Protocol::S_SPAWN_EXISTING_PLAYER& pkt)
{
	// 2. 처음 입장할 떄 이미 있는 플레이어 받기
	int playerNum = pkt.player_size();
	std::cout << "현재 수: " << playerNum << std::endl;
	for (int i = 0; i < playerNum; ++i)
	{
		const Protocol::PlayerInfo& info = pkt.player(i);

		CPlayer* player = new CPlayer(EPlayerAttribute::Fire);

		const Protocol::Vector3& position = info.object_info().pos_info().position();
		player->SetName(L"Player" + std::to_wstring(info.player_id()));
		player->GetTransform()->SetRelativePosition(position.x(), position.y(), position.z());

		CLevelManager::GetInst()->GetCurrentLevel()->AddGameObject(player, 3, false);
		CLevelManager::GetInst()->SetPlayer(player, info.player_id());

		const auto& window = CLevelManager::GetInst()->GetCurrentLevel()->FindWidgetWindow(EWIDGETWINDOW_TYPE::MAP_WINDOW);
		if (window)
		{
			((TestWidget*)window)->AddPlayer(player, info.player_id());
		}
	}
	return true;
}


bool Handle_S_LEAVE_GAME(CPacketSessionRef& session, Protocol::S_LEAVE_GAME& pkt)
{
	std::cout << "======================퇴장======================" << std::endl;
	return true;
}

bool Handle_S_MONSTER_INFO(CPacketSessionRef& session, Protocol::S_MONSTER_INFO& pkt)
{
	CLevel* level = CLevelManager::GetInst()->GetCurrentLevel();
	auto& monsterMap = level->GetLayer(11)->GetMonsterMap();

	for (int i = 0; i < pkt.monster_info_size(); ++i)
	{
		const Protocol::MonsterInfo& info = pkt.monster_info(i);
		uint32_t objectId = info.object_id();

		CMonster* monster = monsterMap[objectId];

		if (nullptr == monster)
		{
			monster = new CMonster();
			monsterMap[objectId] = monster;
			level->SafeAddGameObject(monster, 11, false);
		}

		const Protocol::PosInfo& posInfo = info.object_info().pos_info();
		const Protocol::Vector3& pos = posInfo.position();
		const Protocol::Vector3& rot = posInfo.rotation();
		Protocol::MoveState state = posInfo.state();

		if (state == Protocol::MOVE_STATE_NONE)
		{
			level->GetLayer(11)->SafeRemoveGameObject(monsterMap[objectId]);
			monsterMap.erase(objectId);
			continue;
		}

		// 몬스터 정보 갱신
		monster->SetTarget(Vec3(pos.x(), pos.y(), pos.z()), Vec3(rot.x(), rot.y(), rot.z()));
		monster->SetProtocolStateForClientMonster(state);
		monster->SetStats(info.monster_ablity().maxhp(), info.monster_ablity().hp());
	}
	return true;
}

bool Handle_S_PROJECTILE_INFO(CPacketSessionRef& session, Protocol::S_PROJECTILE_INFO& pkt)
{
	auto& map = CLevelManager::GetInst()->GetCurrentLevel()->GetLayer(12)->GetProjectileMap();
	UINT64 id = pkt.projectile_info().projectile_id();
	if (map.find(id) == map.end())
		return false;

	if ((pkt.mutable_projectile_info()->state()) == Protocol::COLLISION)
	{
		CFireBall* ball = dynamic_cast<CFireBall*>(map[id]);
		Vec3 pos = ball->GetTransform()->GetRelativePosition();
		if (ball)
		{
			if (ball->GetFireParticleObject())
			{
				CParticleSystemManager::GetInst()->Return(ball->GetFireParticleObject());
				ball->SetParticleObject(nullptr);
			}
			if (ball->GetSmokeParticleObject())
			{
				CParticleSystemManager::GetInst()->Return(ball->GetSmokeParticleObject());
				ball->SetSmokeParticleObject(nullptr);
			}

			//if (ball->GetIsBoom())
			{
				CEffectManager::GetInst()->SpawnRadialSmoke(pos);
				CEffectManager::GetInst()->SpawnEffect(L"Explosion", pos);
				CEffectManager::GetInst()->SpawnEffect(L"Explosion1", pos);
				CEffectManager::GetInst()->SpawnEffect(L"Shockwave", pos);
			}
		}

		CLevelManager::GetInst()->GetCurrentLevel()->GetLayer(12)->SafeRemoveGameObject(map[id]);
		map.erase(id);
	}
	else
	{
		const auto& pos = pkt.mutable_projectile_info()->mutable_object_info()->mutable_pos_info()->mutable_position();
		const auto& scale = pkt.mutable_projectile_info()->mutable_object_info()->mutable_pos_info()->mutable_size();
		const auto& rot = pkt.mutable_projectile_info()->mutable_object_info()->mutable_pos_info()->mutable_rotation();
		auto& object = map[id];
		if (object)
		{
			object->GetTransform()->SetRelativePosition(Vec3(pos->x(), pos->y(), pos->z()));
			object->GetTransform()->SetRelativeScale(Vec3(scale->x(), scale->y(), scale->z()));
			object->GetTransform()->SetRelativeRotation(Vec3(rot->x(), rot->y(), rot->z()));
		}
	}
	return true;
}

bool Handle_S_MOVE(CPacketSessionRef& session, Protocol::S_MOVE& pkt)
{
	UINT64 id = pkt.player_move_info().player_id();

	const Protocol::Vector3& position = pkt.player_move_info().pos_info().position();
	const Protocol::Vector3& rotation = pkt.player_move_info().pos_info().rotation();
	Protocol::MoveState state = pkt.player_move_info().pos_info().state();
	CLevelManager::GetInst()->GetPlayer(id)->SetTarget(Vec3(position.x(), position.y(), position.z()), Vec3(rotation.x(), rotation.y(), rotation.z()));

	CLevelManager::GetInst()->GetPlayer(id)->SetProtocolStateForClient(state);
	return true;
}

bool Handle_S_UPDATE_PLAYER(CPacketSessionRef& session, Protocol::S_UPDATE_PLAYER& pkt)
{
	const auto& info = pkt.player_update_info();
	UINT64 id = info.player_id();

	const Protocol::Vector3& position = info.pos_info().position();
	const Protocol::Vector3& rotation = info.pos_info().rotation();
	Protocol::MoveState state = info.pos_info().state();

	const auto& player = CLevelManager::GetInst()->GetPlayer(id);
	player->SetTarget(Vec3(position.x(), position.y(), position.z()), Vec3(rotation.x(), rotation.y(), rotation.z()));
	player->SetProtocolStateForClient(state);

	const auto& stats = info.player_ablity();
	(static_cast<CPlayer*>(player))->SetStats(stats.maxhp(), stats.hp());
	return true;
}

bool Handle_S_DESPAWN_PLAYER(CPacketSessionRef& session, Protocol::S_DESPAWN_PLAYER& pkt)
{
	// 퇴장한 플레이어 데이터 삭제
	UINT64 id = pkt.player_ids();
	std::cout << id << "번 플레이어가 퇴장했음" << std::endl;

	auto player = CLevelManager::GetInst()->GetPlayer(id);
	if (player)
	{
		CLevelManager::GetInst()->GetCurrentLevel()->RemoveGameObject(player);
	}
	CLevelManager::GetInst()->SetPlayer(nullptr, id);
	return true;
}

bool Handle_S_SPAWN(CPacketSessionRef& session, Protocol::S_SPAWN& pkt)
{
	return true;
}


bool Handle_S_DESPAWN(CPacketSessionRef& session, Protocol::S_DESPAWN& pkt)
{
	return true;
}

bool Handle_S_GATE_OPNE(CPacketSessionRef& session, Protocol::S_GATE_OPNE& pkt)
{
	for (int i = 0; i < pkt.open_objects_size(); ++i)
	{
		const auto& info = pkt.open_objects(i);

		CGameObject* object = new CGameObject;
		object->AddComponent(new CTransform);
		object->AddComponent(new CMeshRenderer);
		const auto& posInfo = info.pos_info().position();
		const auto& sizeInfo = info.pos_info().size();
		const auto& rotInfo = info.pos_info().rotation();
		object->m_ObjectId = info.object_id();
		std::wstring name = std::to_wstring(info.object_id());
		object->SetName(name);
		object->GetMeshRenderer()->SetMaterial(CAssetManager::GetInst()->FindAsset<CMaterial>(L"Kita"));
		object->GetMeshRenderer()->SetMesh(CAssetManager::GetInst()->FindAsset<CMesh>(L"Cube"));
		object->GetTransform()->SetRelativePosition(posInfo.x(), posInfo.y(), posInfo.z());
		object->GetTransform()->SetRelativeScale(sizeInfo.x(), sizeInfo.y(), sizeInfo.z());
		object->GetTransform()->SetRelativeRotation(rotInfo.x(), rotInfo.y(), rotInfo.z());
			
		CLevelManager::GetInst()->GetCurrentLevel()->SafeAddGameObject(object, 13, false);
	}
	return true;
}

bool Handle_S_GATE_CLOSE(CPacketSessionRef& session, Protocol::S_GATE_CLOSE& pkt)
{
	const std::vector<CGameObject*>& objects = CLevelManager::GetInst()->GetCurrentLevel()->GetLayer(13)->GetParentObjects();
	for (int i = 0; i < pkt.cloase_objects_size(); ++i)
	{
		std::wstring name = std::to_wstring(pkt.cloase_objects(i).object_id());
		auto iter = std::find_if(objects.begin(), objects.end(), [&](const auto& obj) {
			return obj->GetName() == name;
		});
		
		if (iter != objects.end())
		{
			CLevelManager::GetInst()->GetCurrentLevel()->GetLayer(13)->SafeRemoveGameObject(*iter);
			std::cout << "없애다\n";
		}
		else
		{
			std::cout << "이미 없다\n";
		}
	}


	return true;
}
