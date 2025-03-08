#include "pch.h"
#include "ClientPacketHandler.h"
#include "NetworkManager.h"

std::function<bool(CPacketSessionRef&, BYTE*, int32)> g_ClientPacketHandler[UINT16_MAX];

bool Handle_S_INVALID(CPacketSessionRef& session, BYTE* buffer, int32 len)
{
    std::cout << "Invalid packet" << std::endl;
    return false;
}

bool Handle_S_ENTER_GAME(CPacketSessionRef& session, Protocol::S_ENTER_GAME& pkt)
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

bool Handle_S_MOVE(CPacketSessionRef& session, Protocol::S_MOVE& pkt)
{
    return true;
}
