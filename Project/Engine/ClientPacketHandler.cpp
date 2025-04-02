#include "pch.h"
#include "ClientPacketHandler.h"
#include "Protocol.pb.h"
#include "LevelManager.h"
#include "Player.h"
#include "Level.h"
#include "RenderManager.h"
#include "Camera.h"

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
		Protocol::C_ENTER_GAME enterPkt;

		auto sendBuffer = ClientPacketHandler::MakeSendBuffer(enterPkt);
		session->Send(sendBuffer);
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
	CLevelManager::GetInst()->GetCurrentLevel()->AddGameObject(player, 3, false);
	CLevelManager::GetInst()->SetOwnPlayer(player);
	CLevelManager::GetInst()->SetPlayer(player, id);
	CRenderManager::GetInst()->GetMainCamera()->SetTarget(player);

	return true;
}

bool Handle_S_LEAVE_GAME(CPacketSessionRef& session, Protocol::S_LEAVE_GAME& pkt)
{
	std::cout << "======================퇴장======================" << std::endl;
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

bool Handle_S_SPAWN_NEW_PLAYER(CPacketSessionRef& session, Protocol::S_SPAWN_NEW_PLAYER& pkt)
{
	// 1. 입장한 플레이어 받기
	const Protocol::PlayerInfo& info = pkt.player();

	CPlayer* player = new CPlayer(EPlayerAttribute::Fire);
	CLevelManager::GetInst()->GetCurrentLevel()->AddGameObject(player, 3, false);
	CLevelManager::GetInst()->SetPlayer(player, info.player_id());

	return true;
}

bool Handle_S_SPAWN_EXISTING_PLAYER(CPacketSessionRef& session, Protocol::S_SPAWN_EXISTING_PLAYER& pkt)
{
	// 2. 처음 입장할 떄 이미 있는 플레이어 받기
	int playerNum = pkt.player_size();
	for (int i = 0; i < playerNum; ++i)
	{
		const Protocol::PlayerInfo& info = pkt.player(i);

		CPlayer* player = new CPlayer(EPlayerAttribute::Fire);
		CLevelManager::GetInst()->GetCurrentLevel()->AddGameObject(player, 3, false);
		CLevelManager::GetInst()->SetPlayer(player, info.player_id());
	}
	return true;
}

bool Handle_S_DESPAWN(CPacketSessionRef& session, Protocol::S_DESPAWN& pkt)
{
	return true;
}