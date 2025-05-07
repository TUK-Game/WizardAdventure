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
#include "FireTower.h"
#include "FireCircle.h"
#include "ParticleSystemManager.h"
#include "MeshRenderer.h"
#include "AssetManager.h"
#include "EffectManager.h"
#include "NPC.h"
#include "ItemManager.h"
#include "Item.h"
#include "ItemButtonWidget.h"
#include "PathManager.h"
#include "SkillDataManager.h"
#include "SkillData.h"

#include "SkillButtonWidget.h"
#include "TestWidget.h"
#include "MapPlayerWidget.h"
#include "PlayWidgetWindow.h"
#include "SkillInfo.h"
#include "StoreWidgetWindow.h"
#include "InventoryWIdgetWindow.h"

PacketHandlerFunc g_PacketHandler[UINT16_MAX];

bool Handle_INVALID(CPacketSessionRef& session, BYTE* buffer, int32 len)
{
	return true;
}

bool Handle_S_LOGIN(CPacketSessionRef& session, Protocol::S_LOGIN& pkt)
{
	if (pkt.success())
	{
#ifndef AUTO_SERVER_CONNECT
		Protocol::C_ENTER_GAME enterPkt;
		
		auto sendBuffer = ClientPacketHandler::MakeSendBuffer(enterPkt);
		session->Send(sendBuffer);
#endif
		return true;
	}

	exit(0);

	return false;
}

bool Handle_S_ENTER_GAME(CPacketSessionRef& session, Protocol::S_ENTER_GAME& pkt)
{
	UINT64 id = pkt.player().player_id();

	CPlayer* player{ nullptr };

	switch (pkt.player().player_type())
	{
	case Protocol::PLAYER_TYPE_FIRE:
	{
		player = new CPlayer(EPlayerAttribute::Fire, true, Vec3(11240.f, 0.f, 1127.f));
	}
	break;
	case Protocol::PLAYER_TYPE_ICE:
	{
		player = new CPlayer(EPlayerAttribute::Ice, true, Vec3(11240.f, 0.f, 1127.f));
	}
	break;
	case Protocol::PLAYER_TYPE_LIGHTNING:
	{
		player = new CPlayer(EPlayerAttribute::Electric, true, Vec3(11240.f, 0.f, 1127.f));
	}
	break;
	}

	CLevel* level = CLevelManager::GetInst()->GetCurrentLevel();

	const Protocol::Vector3& position = pkt.player().object_info().pos_info().position();
	player->GetTransform()->SetRelativePosition(position.x(), position.y(), position.z());

	level->AddGameObject(player, LAYER_PLAYER, false);
	CLevelManager::GetInst()->SetOwnPlayer(player);
	CLevelManager::GetInst()->SetPlayer(player, id);
	CRenderManager::GetInst()->GetMainCamera()->SetTarget(player);
	CRenderManager::GetInst()->GetMainCamera()->SetCameraType(ECamera_Type::Fixed);

		
	CNetworkManager::GetInst()->s_GameSession->SetOwnPlayer(player);
	CNetworkManager::GetInst()->s_GameSession->SetClientID(id);

	const auto& mapwindow = level->CreateWidgetWindow<TestWidget>(EWIDGETWINDOW_TYPE::MAP_WINDOW, L"MapWindow", player);
	CPlayWidgetWindow* gamewindow = level->CreateWidgetWindow<CPlayWidgetWindow>(EWIDGETWINDOW_TYPE::GAME_WINDOW, L"GamePlayWidget", player);
	const auto& inventorywindow = level->CreateWidgetWindow<CInventoryWIdgetWindow>(EWIDGETWINDOW_TYPE::INVENTORY_WINDOW, L"InventoryWidget", player);
	const auto& storewindow = level->CreateWidgetWindow<CStoreWidgetWindow>(EWIDGETWINDOW_TYPE::STORE_WINDOW, L"StoreWindow", nullptr);

	if (storewindow)
	{
		storewindow->SetEnable(false);
	}
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
	if (inventorywindow)
	{
		inventorywindow->SetEnable(false);
	}

	Protocol::C_ENTER_GAME_SUCCESS GSpkt;
	switch (player->GetAttribute())
	{
	case EPlayerAttribute::Fire:
	{
		GSpkt.mutable_player()->set_player_type(Protocol::PLAYER_TYPE_FIRE);
		player->InitStats(100, 100, 30, 300.f);
		// temp

		player->GetSkillManager()->LearnSkill(ESkillSlot::LButton, CSkillDataManager::GetInst()->FindSkill(CSkillDataManager::GetInst()->FindSkillId(L"FireShot")));
		player->GetSkillManager()->LearnSkill(ESkillSlot::RButton, CSkillDataManager::GetInst()->FindSkill(CSkillDataManager::GetInst()->FindSkillId(L"FireRain")));
		//player->GetSkillManager()->LearnSkill(ESkillSlot::Q, ESkillType::FireBallTowardQ);
		//player->GetSkillManager()->LearnSkill(ESkillSlot::E, ESkillType::FireTower);
		//player->GetSkillManager()->LearnSkill(ESkillSlot::R, ESkillType::FireSwordSpread);

		gamewindow->SetSkill(ESkillType::FireBallTowardMouse, Skill::FireBall.cooldown, ESkillSlot::LButton);
		gamewindow->SetSkill(ESkillType::Meteor, Skill::Meteor.cooldown, ESkillSlot::RButton);
		// gamewindow->SetSkill(ESkillType::FireBallTowardQ, Skill::FireBallQ.cooldown, ESkillSlot::Q);
		// gamewindow->SetSkill(ESkillType::FireTower, Skill::FireTower.cooldown, ESkillSlot::E);
		// gamewindow->SetSkill(ESkillType::FireSwordSpread, Skill::FireSword.cooldown, ESkillSlot::R);
		gamewindow->SetGauge(L"HPBar", 100, true);
		gamewindow->SetGauge(L"SignautreGage", 0, false);

	}
	break;
	case EPlayerAttribute::Ice:
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

bool Handle_S_SPAWN_NEW_PLAYER(CPacketSessionRef& session, Protocol::S_SPAWN_NEW_PLAYER& pkt)
{
	const Protocol::PlayerInfo& info = pkt.player();

	CPlayer* player{nullptr};

	switch (pkt.player().player_type())
	{
	case Protocol::PLAYER_TYPE_FIRE:
	{
		player = new CPlayer(EPlayerAttribute::Fire, false, Vec3(11240.f, 0.f, 1127.f));
		player->InitStats(100, 100, 30, 300.f);

		player->GetSkillManager()->LearnSkill(ESkillSlot::LButton, CSkillDataManager::GetInst()->FindSkill(CSkillDataManager::GetInst()->FindSkillId(L"FireShot")));
		player->GetSkillManager()->LearnSkill(ESkillSlot::RButton, CSkillDataManager::GetInst()->FindSkill(CSkillDataManager::GetInst()->FindSkillId(L"FireRain")));
		//player->GetSkillManager()->LearnSkill(ESkillSlot::Q, ESkillType::FireBallTowardQ);
		//player->GetSkillManager()->LearnSkill(ESkillSlot::E, ESkillType::FireTower);
		//player->GetSkillManager()->LearnSkill(ESkillSlot::R, ESkillType::FireSwordSpread);
	}
	break;
	case Protocol::PLAYER_TYPE_ICE:
	{
		player = new CPlayer(EPlayerAttribute::Ice, false, Vec3(11240.f, 0.f, 1127.f));
	}
	break;
	case Protocol::PLAYER_TYPE_LIGHTNING:
	{
		player = new CPlayer(EPlayerAttribute::Electric, false, Vec3(11240.f, 0.f, 1127.f));
	}
	break;
	}

	const Protocol::Vector3& position = pkt.player().object_info().pos_info().position();
	player->SetName(L"Player" + std::to_wstring(info.player_id()));
	player->GetTransform()->SetRelativePosition(position.x(), position.y(), position.z());
	CLevelManager::GetInst()->GetCurrentLevel()->AddGameObject(player, LAYER_PLAYER, false);
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
	int playerNum = pkt.player_size();
	for (int i = 0; i < playerNum; ++i)
	{
		const Protocol::PlayerInfo& info = pkt.player(i);

		CPlayer* player{ nullptr };
		const Protocol::Vector3& position = info.object_info().pos_info().position();
		switch (info.player_type())
		{
		case Protocol::PLAYER_TYPE_FIRE:
		{
			player = new CPlayer(EPlayerAttribute::Fire, false, Vec3(position.x(), position.y(), position.z()));
			player->InitStats(100, 100, 30, 300.f);

			player->GetSkillManager()->LearnSkill(ESkillSlot::LButton, CSkillDataManager::GetInst()->FindSkill(CSkillDataManager::GetInst()->FindSkillId(L"FireShot")));
			player->GetSkillManager()->LearnSkill(ESkillSlot::RButton, CSkillDataManager::GetInst()->FindSkill(CSkillDataManager::GetInst()->FindSkillId(L"FireRain")));
			//player->GetSkillManager()->LearnSkill(ESkillSlot::Q, ESkillType::FireBallTowardQ);
			//player->GetSkillManager()->LearnSkill(ESkillSlot::E, ESkillType::FireTower);
			//player->GetSkillManager()->LearnSkill(ESkillSlot::R, ESkillType::FireSwordSpread);
		}
		break;
		case Protocol::PLAYER_TYPE_ICE:
		{
			player = new CPlayer(EPlayerAttribute::Ice, false, Vec3(position.x(), position.y(), position.z()));
		}
		break;
		case Protocol::PLAYER_TYPE_LIGHTNING:
		{
			player = new CPlayer(EPlayerAttribute::Electric, false, Vec3(position.x(), position.y(), position.z()));
		}
		break;
		}

		const Protocol::Vector3& rotation = info.object_info().pos_info().rotation();

		player->SetName(L"Player" + std::to_wstring(info.player_id()));
		player->GetTransform()->SetRelativePosition(position.x(), position.y(), position.z());
		player->GetTransform()->SetRelativeRotation(rotation.x(), rotation.y(), rotation.z());

		CLevelManager::GetInst()->GetCurrentLevel()->AddGameObject(player, LAYER_PLAYER, false);
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
	return true;
}

bool Handle_S_MONSTER_INFO(CPacketSessionRef& session, Protocol::S_MONSTER_INFO& pkt)
{
	CLevel* level = CLevelManager::GetInst()->GetCurrentLevel();
	auto& monsterMap = level->GetLayer(LAYER_MONSTER)->GetMonsterMap();

	for (int i = 0; i < pkt.monster_info_size(); ++i)
	{
		const Protocol::MonsterInfo& info = pkt.monster_info(i);
		uint32_t objectId = info.object_id();

		CMonster* monster = monsterMap[objectId];

		if (nullptr == monster)
		{
			monster = new CMonster();
			monsterMap[objectId] = monster;
			level->SafeAddGameObject(monster, LAYER_MONSTER, false);
		}

		const Protocol::PosInfo& posInfo = info.object_info().pos_info();
		const Protocol::Vector3& pos = posInfo.position();
		const Protocol::Vector3& rot = posInfo.rotation();
		Protocol::MoveState state = posInfo.state();

		if (state == Protocol::MOVE_STATE_NONE)
		{
			level->GetLayer(LAYER_MONSTER)->SafeRemoveGameObject(monsterMap[objectId]);
			monsterMap.erase(objectId);
			continue;
		}

		// ���� ���� ����
		monster->SetTarget(Vec3(pos.x(), pos.y(), pos.z()), Vec3(rot.x(), rot.y(), rot.z()));
		monster->SetProtocolStateForClientMonster(state);
		monster->SetStats(info.monster_ablity().maxhp(), info.monster_ablity().hp());
	}
	return true;
}

bool Handle_S_SPAWN_PROJECTILE_SUCESSE(CPacketSessionRef& session, Protocol::S_SPAWN_PROJECTILE_SUCESSE& pkt)
{
	UINT64 id = pkt.projectile_id();
	auto& map = CLevelManager::GetInst()->GetCurrentLevel()->GetLayer(LAYER_PROJECTILE)->GetProjectileMap();
	if (map.find(id) != map.end())
	{
		std::cout << "생성\n";
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
			CLevelManager::GetInst()->GetCurrentLevel()->SafeAddGameObject(magic, LAYER_PROJECTILE, false);
		}
		break;
		case Protocol::FIRE_BALL:
		{
			CFireBall* magic = new CFireBall();
			magic->GetTransform()->SetRelativeScale(Vec3(size->x(), size->y(), size->z()));
			magic->SetMode(EFireBallMode::Default);
			map[id] = magic;
			CLevelManager::GetInst()->GetCurrentLevel()->SafeAddGameObject(magic, LAYER_PROJECTILE, false);
		}
		break;
		case Protocol::FIRE_BALL_EXPLOSION:
		{
			CFireBall* magic = new CFireBall();
			magic->GetTransform()->SetRelativeScale(Vec3(size->x(), size->y(), size->z()));
			magic->SetMode(EFireBallMode::QSkill);
			map[id] = magic;
			CLevelManager::GetInst()->GetCurrentLevel()->SafeAddGameObject(magic, LAYER_PROJECTILE, false);
		}
		break;
		case Protocol::FIRE_METEOR:
		{
			CFireBall* magic = new CFireBall();
			magic->GetTransform()->SetRelativeScale(Vec3(size->x(), size->y(), size->z()));
			magic->SetMode(EFireBallMode::Meteor);
			map[id] = magic;
			magic->UseSmokeTrail();
			CLevelManager::GetInst()->GetCurrentLevel()->SafeAddGameObject(magic, LAYER_PROJECTILE, false);
		}
		break;
		case Protocol::FIRE_PILLAR:
		{
			CFireTower* magic = new CFireTower();
			magic->GetTransform()->SetRelativeScale(Vec3(size->x(), size->y(), size->z()));
			map[id] = magic;
			CLevelManager::GetInst()->GetCurrentLevel()->SafeAddGameObject(magic, LAYER_PROJECTILE, false);
		}
		break;
		case Protocol::FIRE_SWORD:
		{
			CFireSword* magic = new CFireSword();
			magic->GetTransform()->SetRelativeScale(Vec3(size->x(), size->y(), size->z()));
			map[id] = magic;
			CLevelManager::GetInst()->GetCurrentLevel()->SafeAddGameObject(magic, LAYER_PROJECTILE, false);
		}
		break;
		}

	}
	return true;
}

bool Handle_S_PROJECTILE_INFO(CPacketSessionRef& session, Protocol::S_PROJECTILE_INFO& pkt)
{
	auto& map = CLevelManager::GetInst()->GetCurrentLevel()->GetLayer(LAYER_PROJECTILE)->GetProjectileMap();
	UINT64 id = pkt.projectile_info().projectile_id();
	if (map.find(id) == map.end())
		return false;

	if ((pkt.projectile_info().state()) == Protocol::COLLISION)
	{
		map[id]->OffParticles();
		CLevelManager::GetInst()->GetCurrentLevel()->GetLayer(LAYER_PROJECTILE)->SafeRemoveGameObject(map[id]);
		map.erase(id);
	}
	else
	{
		const auto& pos = pkt.projectile_info().object_info().pos_info().position();
		const auto& scale = pkt.projectile_info().object_info().pos_info().size();
		const auto& rot = pkt.projectile_info().object_info().pos_info().rotation();
		auto& object = map[id];
		if (object)
		{
			object->GetTransform()->SetRelativePosition(Vec3(pos.x(), pos.y(), pos.z()));
			object->GetTransform()->SetRelativeScale(Vec3(scale.x(), scale.y(), scale.z()));
			object->GetTransform()->SetRelativeRotation(Vec3(rot.x(), rot.y(), rot.z()));
		}
	}
	return true;
}

bool Handle_S_PROJECTILE_EFFECT(CPacketSessionRef& session, Protocol::S_PROJECTILE_EFFECT& pkt)
{
	auto& map = CLevelManager::GetInst()->GetCurrentLevel()->GetLayer(LAYER_PROJECTILE)->GetProjectileMap();
	UINT64 id = pkt.projectile_id();
	if (map.find(id) == map.end())
		return false;

	map[id]->ShowParticles();
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

bool Handle_S_ACT(CPacketSessionRef& session, Protocol::S_ACT& pkt)
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
	UINT64 id = pkt.player_ids();

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
			
		CLevelManager::GetInst()->GetCurrentLevel()->SafeAddGameObject(object, LAYER_GATE, false);
	}
	return true;
}

bool Handle_S_GATE_CLOSE(CPacketSessionRef& session, Protocol::S_GATE_CLOSE& pkt)
{
	const std::vector<CGameObject*>& objects = CLevelManager::GetInst()->GetCurrentLevel()->GetLayer(LAYER_GATE)->GetParentObjects();
	for (int i = 0; i < pkt.cloase_objects_size(); ++i)
	{
		std::wstring name = std::to_wstring(pkt.cloase_objects(i).object_id());
		auto iter = std::find_if(objects.begin(), objects.end(), [&](const auto& obj) {
			return obj->GetName() == name;
		});
		
		if (iter != objects.end())
		{
			CLevelManager::GetInst()->GetCurrentLevel()->GetLayer(LAYER_GATE)->SafeRemoveGameObject(*iter);
		}
	}


	return true;
}

bool Handle_S_SPAWN_NPC(CPacketSessionRef& session, Protocol::S_SPAWN_NPC& pkt)
{
	CLevel* level = CLevelManager::GetInst()->GetCurrentLevel();
	const auto& win = level->FindWidgetWindow(EWIDGETWINDOW_TYPE::STORE_WINDOW);
	win->SetEnable(false);

	for (int i = 0; i < pkt.npc_info_size(); ++i)
	{
		const Protocol::NpcInfo& info = pkt.npc_info(i);
		CNPC* npc = new CNPC;
		const auto& posInfo = info.object_info().pos_info().position();
		const auto& sizeInfo = info.object_info().pos_info().size();
		const auto& rotInfo = info.object_info().pos_info().rotation();
		npc->m_ObjectId = info.object_id();
		std::wstring name = std::to_wstring(info.object_id());
		npc->SetName(name);
		npc->GetTransform()->SetRelativePosition(posInfo.x(), posInfo.y(), posInfo.z());
		npc->GetTransform()->SetRelativeRotation(rotInfo.x(), rotInfo.y(), rotInfo.z());
		npc->SetWidgetWindowType(EWIDGETWINDOW_TYPE::STORE_WINDOW);

		for (int j = 0; j < info.item_id_size(); ++j)
		{
			uint32 id = info.item_id(j);
			const auto& item = CItemManager::GetInst()->FindItem(id);

			std::wstring itemName = item->GetItemInfo().name;
			CTexture* texture = CAssetManager::GetInst()->FindAsset<CTexture>(itemName);
			if (!texture)
			{
				auto path = CPathManager::GetInst()->FindPath(TEXTURE_PATH);
				std::wstring fullPath = path / (L"Item\\" + itemName + L".png");
				texture = new CTexture;
				texture->Init(fullPath);
				CAssetManager::GetInst()->AddAsset(itemName, texture);
			}

			CItemButtonWidget* widget = dynamic_cast<CItemButtonWidget*>(win->FindWidget(L"Item" + std::to_wstring(j + 1)));
			if(widget)
			{
				widget->SetItemId(id);
				widget->SetItem(item);
				widget->SetButtonTexture(
					CAssetManager::GetInst()->FindAsset<CTexture>(itemName),
					CAssetManager::GetInst()->FindAsset<CTexture>(itemName),
					CAssetManager::GetInst()->FindAsset<CTexture>(itemName)
				);
			}
		}

		for (int j = 0; j < info.skill_id_size(); ++j)
		{
			uint32 id = info.skill_id(j);
			const auto& skill = CSkillDataManager::GetInst()->FindSkill(id);

			std::wstring skillName = skill->GetSkillInfo().name;
			CTexture* texture = CAssetManager::GetInst()->FindAsset<CTexture>(skillName);

			CSkillButtonWidget* widget = dynamic_cast<CSkillButtonWidget*>(win->FindWidget(L"NewSkill" + std::to_wstring(j + 1)));
			if (widget)
			{
				widget->SetSkillId(id);
				widget->SetSkill(skill);
				widget->SetButtonTexture(
					CAssetManager::GetInst()->FindAsset<CTexture>(skillName),
					CAssetManager::GetInst()->FindAsset<CTexture>(skillName),
					CAssetManager::GetInst()->FindAsset<CTexture>(skillName)
				);
			}
		}

		npc->Begin();
		CLevelManager::GetInst()->GetCurrentLevel()->SafeAddGameObject(npc, LAYER_NPC, false);
	}

	return true;
}

bool Handle_S_UPDATE_ITEM(CPacketSessionRef& session, Protocol::S_UPDATE_ITEM& pkt)
{
	CLevel* level = CLevelManager::GetInst()->GetCurrentLevel();
	const auto& win = level->FindWidgetWindow(EWIDGETWINDOW_TYPE::STORE_WINDOW);

	for (int i = 0; i < pkt.item_info_size(); ++i)
	{
		const Protocol::ItemInfo& info = pkt.item_info(i);

		CItemButtonWidget* widget = dynamic_cast<CItemButtonWidget*>(win->FindWidget(L"Item" + std::to_wstring(i + 1)));
		if (widget && widget->GetEnable())
		{
			widget->SetEnable(!info.is_sell());
		}
	}

	for (int i = 0; i < pkt.skill_info_size(); ++i)
	{
		const Protocol::SkillInfo& info = pkt.skill_info(i);

		CSkillButtonWidget* widget = dynamic_cast<CSkillButtonWidget*>(win->FindWidget(L"NewSkill" + std::to_wstring(i + 1)));
		if (widget && widget->GetEnable())
		{
			widget->SetEnable(!info.is_sell());
		}
	}
	return true;
}

bool Handle_S_BUY_ITEM(CPacketSessionRef& session, Protocol::S_BUY_ITEM& pkt)
{
	if (pkt.is_success())
	{
		UINT32 playerId = pkt.player_id();
		UINT32 itemId = pkt.item_id();

		const auto& objects = CLevelManager::GetInst()->GetCurrentLevel()->GetLayer(LAYER_NPC)->GetParentObjects();
		CNPC* npc = dynamic_cast<CNPC*>(objects[0]);
		if (npc)
		{
			npc->SuccessInteration();
		}
		const auto& item = CItemManager::GetInst()->FindItem(itemId);
		CPlayer* player = dynamic_cast<CPlayer*>(CLevelManager::GetInst()->GetPlayer(playerId));
		CInventoryWIdgetWindow* inven = dynamic_cast<CInventoryWIdgetWindow*>(CLevelManager::GetInst()->GetCurrentLevel()->FindWidgetWindow(EWIDGETWINDOW_TYPE::INVENTORY_WINDOW));
		if(item && player && inven)
		{
			player->AddItem(item);
			inven->UpdateInventory();
		}
	}
	return true;
}

bool Handle_S_BUY_SKILL(CPacketSessionRef& session, Protocol::S_BUY_SKILL& pkt)
{
	if (pkt.is_success())
	{
		UINT32 playerId = pkt.player_id();
		UINT32 skillId = pkt.skill_id();

		const auto& objects = CLevelManager::GetInst()->GetCurrentLevel()->GetLayer(LAYER_NPC)->GetParentObjects();
		CNPC* npc = dynamic_cast<CNPC*>(objects[0]);
		if (npc)
		{
			npc->SuccessInteration();
		}
		const auto& skill = CSkillDataManager::GetInst()->FindSkill(skillId);
		CPlayer* player = dynamic_cast<CPlayer*>(CLevelManager::GetInst()->GetPlayer(playerId));
		CInventoryWIdgetWindow* inven = dynamic_cast<CInventoryWIdgetWindow*>(CLevelManager::GetInst()->GetCurrentLevel()->FindWidgetWindow(EWIDGETWINDOW_TYPE::INVENTORY_WINDOW));
		CPlayWidgetWindow* gamewindow = dynamic_cast<CPlayWidgetWindow*>(CLevelManager::GetInst()->GetCurrentLevel()->FindWidgetWindow(EWIDGETWINDOW_TYPE::GAME_WINDOW));
		if (skill && player && inven && gamewindow)
		{
			const auto& skillManager = player->GetSkillManager();
			if(gamewindow->GetOwnerPlayer() == player)
			{
				gamewindow->SetSkill(ConvertSkillNameToType(skill->GetSkillInfo().name), skill->GetSkillInfo().cooltime, player->GetSkillManager()->GetNextSlot());
			}
			player->AddSkill(skill);
			inven->UpdateInventory();
		}
	}
	return true; 
}