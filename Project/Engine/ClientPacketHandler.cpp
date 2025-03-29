#include "pch.h"
#include "ClientPacketHandler.h"
#include "Protocol.pb.h"

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
		//Protocol::C_ENTER_GAME enterPkt;
		//auto sendBuffer = ClientPacketHandler::MakeSendBuffer(enterPkt);
		//session->Send(sendBuffer);
		return true;
	}

	std::cout << "로그인 실패!" << std::endl;
	exit(0);

	return false;
}

bool Handle_S_ENTER_GAME(CPacketSessionRef& session, Protocol::S_ENTER_GAME& pkt)
{
	return true;
}

bool Handle_S_LEAVE_GAME(CPacketSessionRef& session, Protocol::S_LEAVE_GAME& pkt)
{
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