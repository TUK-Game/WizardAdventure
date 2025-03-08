#include "pch.h"
#include "ServerPacketHandler.h"
#include "NetworkManager.h"
#include "ClientSession.h"

std::function<bool(CPacketSessionRef&, BYTE*, int32)> g_ServerPacketHandler[UINT16_MAX];

bool Handle_C_INVALID(CPacketSessionRef& session, BYTE* buffer, int32 len)
{
	std::cout << "Invalid packet" << std::endl;
	return false;
}

bool Handle_C_ENTER_GAME(CPacketSessionRef& session, Protocol::C_ENTER_GAME& pkt)
{
	std::cout << "Recv Packet C_ENTER_GAME" << std::endl;

	return true;
}

bool Handle_C_MOVE(CPacketSessionRef& session, Protocol::C_MOVE& pkt)
{
	return true;
}
