#include "pch.h"
#include "ServerPacketHandler.h"
#include "Protocol.pb.h"
#include "Room.h"
#include "ObjectUtil.h"
#include "Player.h"
#include "GameSession.h"
#include "Projectile.h"
#include "ProjectilePool.h"

PacketHandlerFunc g_PacketHandler[UINT16_MAX];


bool Handle_INVALID(CPacketSessionRef& session, BYTE* buffer, int32 len)
{
	PacketHeader* header = reinterpret_cast<PacketHeader*>(buffer);
	// TODO: 로그
	return false;
}

bool Handle_C_LOGIN(CPacketSessionRef& session, Protocol::C_LOGIN& pkt)
{
	Protocol::S_LOGIN loginPkt;

	loginPkt.set_success(true);

	SEND_PACKET(loginPkt);

	return true;
}

bool Handle_C_ENTER_GAME(CPacketSessionRef& session, Protocol::C_ENTER_GAME& pkt)
{
	// 플레이어 생성
	CPlayerRef player = CObjectUtil::CreatePlayer(static_pointer_cast<CGameSession>(session));

	// 방에 입장
	g_Room->DoAsync(&CRoom::HandleEnterPlayer, player);

	return true;
}

bool Handle_C_ENTER_GAME_SUCCESS(CPacketSessionRef& session, Protocol::C_ENTER_GAME_SUCCESS& pkt)
{
	// 속성별 능력치 초기화 및 클라에 전송 -> 전송은 클라가 첨에 값 넣고 중력검사 떄 같이 보내서 동기화하면 ㄱㅊ할 듯
	uint32 id = pkt.mutable_player()->player_id();
	auto gameSession = static_pointer_cast<CGameSession>(session);
	CPlayerRef player = gameSession->Player.load();
	if (player == nullptr)
		return false;
	
	const auto type = pkt.mutable_player()->player_type();
	switch (type)
	{
	case Protocol::PLAYER_TYPE_FIRE:
	{
		player->SetAttribute(EAttribution::FIRE);
		player->SetAblity(30, 100, 30, 300.f);
	}
	break;
	case Protocol::PLAYER_TYPE_ICE:
	{
		player->SetAttribute(EAttribution::ICE);
		//player->SetAblity(30, 100, 30, 300.f);
	}
	break;
	case Protocol::PLAYER_TYPE_LIGHTNING:
	{
		player->SetAttribute(EAttribution::LIGHTNING);
		//player->SetAblity(30, 100, 30, 300.f);
	}
	break;
	default:
		break;
	}


	return true;
}

bool Handle_C_LEAVE_GAME(CPacketSessionRef& session, Protocol::C_LEAVE_GAME& pkt)
{
	auto gameSession = static_pointer_cast<CGameSession>(session);

	CPlayerRef player = gameSession->Player.load();
	if (player == nullptr)
		return false;

	CRoomRef room = player->GetRoom();
	if (room == nullptr)
		return false;

	g_Room->DoAsync(&CRoom::HandleLeavePlayer, player);

	return true;
}

bool Handle_C_MOVE(CPacketSessionRef& session, Protocol::C_MOVE& pkt)
{
	// TODO 플레이어 스텟 업데이트 - 

	auto gameSession = static_pointer_cast<CGameSession>(session);
	CPlayerRef player = gameSession->Player.load();
	if (player == nullptr)
		return false;

	bool IsMove = pkt.ismove();
	const auto& pos = pkt.player_move_info().pos_info().position();

	const auto& rot = pkt.player_move_info().pos_info().rotation();
	const auto& dir = pkt.dir();
	auto state = pkt.player_move_info().pos_info().state();

	player->m_NextAmount.CopyFrom(pos);

	auto* rotProto = player->PlayerInfo->mutable_object_info()->mutable_pos_info()->mutable_rotation();
	rotProto->set_x(rot.x());
	rotProto->set_y(rot.y());
	rotProto->set_z(rot.z());

	player->SetDir(dir);
	player->SetState(state);

	if (IsMove)
	{
		g_Room->DoAsync(&CRoom::HandleMovePlayer, player);
	}
	return true;
}

bool Handle_C_SPAWN_PROJECTILE(CPacketSessionRef& session, Protocol::C_SPAWN_PROJECTILE& pkt)
{
	const auto& info = pkt.info();

	CProjectileRef projectile = g_pool->Allocate();
	assert(projectile != nullptr);

	ProjectileState state;
	state.Direction = Vec3(info.dir().x(), info.dir().y(), info.dir().z());
	state.Size = Vec3(info.size().x(), info.size().y(), info.size().z());
	state.Speed = info.speed();
	state.ElapsedTime = info.duration();
	state.damage = info.damage();


	projectile->SetProjectileState(state);
	projectile->ProjectileInfo->set_projectile_id(info.new_projectile_id());
	projectile->SetCollisionExplosion(pkt.info().bcollisionexplosion());
	projectile->ProjectileInfo->mutable_object_info()->mutable_pos_info()->mutable_position()->set_x(info.spawn_pos().x());
	projectile->ProjectileInfo->mutable_object_info()->mutable_pos_info()->mutable_position()->set_y(info.spawn_pos().y());
	projectile->ProjectileInfo->mutable_object_info()->mutable_pos_info()->mutable_position()->set_z(info.spawn_pos().z());
	projectile->ProjectileInfo->set_state(Protocol::MOVE_STATE);
	projectile->SetCollisionBoxInfo(Vec3(info.spawn_pos().x(), info.spawn_pos().y(), info.spawn_pos().z()), state.Size, Vec3(0.f, 0.f, 0.f));
	projectile->m_meshType = pkt.mesh();
	g_Room->DoAsync(&CRoom::HandleSpawnProjectile, projectile);
	return true;
}

bool Handle_C_MOVE_PROJECTILE(CPacketSessionRef& session, Protocol::C_MOVE_PROJECTILE& pkt)
{
	const auto& info = pkt.projectile_info();
	
	int projectile_id = info.projectile_id();
	
	CProjectileRef object = std::dynamic_pointer_cast<CProjectile>(g_Room->GetLayerObject((uint32)EObject_Type::Projectile, projectile_id));
	if (object == nullptr)
		return true;
	const auto& posInfo = pkt.mutable_projectile_info()->mutable_object_info()->mutable_pos_info()->position();
	const auto& scaleInfo = pkt.mutable_projectile_info()->mutable_object_info()->mutable_pos_info()->size();
	const auto& rotInfo = pkt.mutable_projectile_info()->mutable_object_info()->mutable_pos_info()->rotation();
	const auto state = pkt.projectile_info().state();
	
	auto* pos = object->ProjectileInfo->mutable_object_info()->mutable_pos_info()->mutable_position();
	auto* scale = object->ProjectileInfo->mutable_object_info()->mutable_pos_info()->mutable_size();
	auto* rot = object->ProjectileInfo->mutable_object_info()->mutable_pos_info()->mutable_rotation();
	pos->set_x(posInfo.x());
	pos->set_y(posInfo.y());
	pos->set_z(posInfo.z());

	scale->set_x(scaleInfo.x());
	scale->set_y(scaleInfo.y());
	scale->set_z(scaleInfo.z());

	rot->set_x(rotInfo.x());
	rot->set_y(rotInfo.y());
	rot->set_z(rotInfo.z());

	if(object->ProjectileInfo->state() != Protocol::COLLISION)
	{
		object->ProjectileInfo->set_state(state);
	}
	
	g_Room->DoAsync(&CRoom::HandleMoveProjectile, object);
	return true;
}
