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
	projectile->ProjectileInfo->set_projectile_id(g_pool->GetIdx());
	projectile->ProjectileInfo->mutable_object_info()->mutable_pos_info()->mutable_position()->set_x(info.spawn_pos().x());
	projectile->ProjectileInfo->mutable_object_info()->mutable_pos_info()->mutable_position()->set_y(info.spawn_pos().y());
	projectile->ProjectileInfo->mutable_object_info()->mutable_pos_info()->mutable_position()->set_z(info.spawn_pos().z());
	projectile->SetCollisionBoxInfo(Vec3(info.spawn_pos().x(), info.spawn_pos().y(), info.spawn_pos().z()), state.Size, Vec3(0.f, 0.f, 0.f));
	g_Room->DoAsync(&CRoom::HandleSpawnProjectile, projectile);
	std::cout << "발사!" << info.player_id() << "가 쏨\n";
	return true;
}

bool Handle_C_MOVE(CPacketSessionRef& session, Protocol::C_MOVE& pkt)
{
	// TODO - 움직임 업데이트 로직 및 패킷 재전송
	// 1. 움직임 업데이트 -> 룸에서 업데이트 하도록 실행
	// 2. 플레이어 위치정보 포장해서 재전송
	auto gameSession = static_pointer_cast<CGameSession>(session);
	CPlayerRef player = gameSession->Player.load();
	if (player == nullptr)
		return false;

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

	g_Room->DoAsync(&CRoom::HandleMovePlayer, player);

	return true;
}

bool Handle_C_MOVE_PROJECTILE(CPacketSessionRef& session, Protocol::C_MOVE_PROJECTILE& pkt)
{
	const auto& info = pkt.projectile_info();

	int projectile_id = info.projectile_id();

	CProjectileRef object = std::dynamic_pointer_cast<CProjectile>(g_Room->GetLayerObject((uint32)EObject_Type::Projectile, projectile_id));

	const auto& posInfo = pkt.mutable_projectile_info()->mutable_object_info()->mutable_pos_info()->position();
	const auto state = pkt.projectile_info().state();

	auto* pos = object->ProjectileInfo->mutable_object_info()->mutable_pos_info()->mutable_position();
	pos->set_x(posInfo.x());
	pos->set_y(posInfo.y());
	pos->set_z(posInfo.z());

	object->ProjectileInfo->set_state(state);

	g_Room->DoAsync(&CRoom::HandleMoveProjectile, object);
	return true;
}
