#include "pch.h"
#include "GameSession.h"
#include "GameSessionManager.h"
#include "ServerPacketHandler.h"
#include "Player.h"

CGameSession::CGameSession()
	: Id(-1)
{
}

CGameSession::~CGameSession()
{
	std::cout << "~GameSession" << std::endl;
}

void CGameSession::OnConnected()
{
	Id = g_SessionManager.Add(static_pointer_cast<CGameSession>(shared_from_this()));
	std::cout << "Connected Player " << Id << std::endl;
}

void CGameSession::OnDisconnected()
{
	g_SessionManager.Remove(static_pointer_cast<CGameSession>(shared_from_this()));
	std::cout << "Disconnected Player " << Id << std::endl;
}

void CGameSession::OnRecvPacket(BYTE* buffer, int32 len)
{
	CPacketSessionRef session = GetPacketSessionRef();
	PacketHeader* header = reinterpret_cast<PacketHeader*>(buffer);

	// TODO: packetId 대역 체크
	ServerPacketHandler::HandlePacket(session, buffer, len);
}

void CGameSession::OnSend(int32 len)
{
}