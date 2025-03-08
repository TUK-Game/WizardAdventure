#include "pch.h"
#include "ClientSession.h"
#include "ClientSessionManager.h"
#include "ServerPacketHandler.h"
#include "LevelManager.h"
#include "Level.h"
#include "NetworkManager.h"

CClientSession::CClientSession()
	: m_Id(-1)
{
}

CClientSession::~CClientSession()
{
	std::cout << "~ClientSession" << std::endl;
}

void CClientSession::OnConnected()
{
	m_Id = CClientSessionManager::GetInst()->Add(static_pointer_cast<CClientSession>(shared_from_this()));
	std::cout << "Connect : Player " << m_Id << std::endl;
}

void CClientSession::OnDisconnected()
{
	CClientSessionManager::GetInst()->Remove(static_pointer_cast<CClientSession>(shared_from_this()));
	std::cout << "Disconnect : Player " << m_Id << std::endl;
}

void CClientSession::OnRecvPacket(BYTE* buffer, int32 len)
{
	CPacketSessionRef session = GetPacketSessionRef();
	PacketHeader* header = reinterpret_cast<PacketHeader*>(buffer);

	ServerPacketHandler::HandlePacket(session, buffer, len);
}

void CClientSession::OnSend(int32 len)
{
	//std::cout << "OnSend Len - " << len << std::endl;
}
