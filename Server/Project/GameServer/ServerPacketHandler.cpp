#include "pch.h"
#include "ServerPacketHandler.h"
#include "Protocol.pb.h"
#include "Room.h"
#include "ObjectUtil.h"
#include "Player.h"
#include "GameSession.h"

PacketHandlerFunc g_PacketHandler[UINT16_MAX];

bool Handle_INVALID(CPacketSessionRef& session, BYTE* buffer, int32 len)
{
	PacketHeader* header = reinterpret_cast<PacketHeader*>(buffer);
	// TODO: �α�
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
	// �÷��̾� ����
	CPlayerRef player = CObjectUtil::CreatePlayer(static_pointer_cast<CGameSession>(session));

	// �濡 ����
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

bool Handle_C_MOVE(CPacketSessionRef& session, Protocol::C_MOVE& pkt)
{
	std::cout << "������ ����!!!!!!!!!!!!!" << std::endl;
	// TODO - ������ ������Ʈ ���� �� ��Ŷ ������
	// 1. ������ ������Ʈ -> �뿡�� ������Ʈ �ϵ��� ����
	// 2. �÷��̾� ��ġ���� �����ؼ� ������
	auto gameSession = static_pointer_cast<CGameSession>(session);

	CPlayerRef player = gameSession->Player.load();
	if (player == nullptr)
		return false;

	const Protocol::Vector3& pos = pkt.player_move_info().pos_info().position();
	player->PlayerInfo->mutable_object_info()->mutable_pos_info()->mutable_position()->set_x(pos.x());
	player->PlayerInfo->mutable_object_info()->mutable_pos_info()->mutable_position()->set_y(pos.y());
	player->PlayerInfo->mutable_object_info()->mutable_pos_info()->mutable_position()->set_z(pos.z());


	g_Room->DoAsync(&CRoom::HandleMovePlayer, player);

	return true;
}
