#include "pch.h"
#include "ServerSession.h"
#include "ClientPacketHandler.h"
#include "Player.h"
#include "Transform.h"

CServerSession::CServerSession()
{
}

CServerSession::~CServerSession()
{
	std::cout << "~ServerSession" << std::endl;
}

void CServerSession::OnConnected()
{
	std::cout << "Connected Server" << std::endl;

	Protocol::C_LOGIN pkt;	
	std::shared_ptr<CSendBuffer> SendBuffer = ClientPacketHandler::MakeSendBuffer(pkt);
	Send(SendBuffer);
}

void CServerSession::OnDisconnected()
{
	std::cout << "Disconnected Server" << std::endl;

	Protocol::C_LEAVE_GAME pkt;
	std::shared_ptr<CSendBuffer> SendBuffer = ClientPacketHandler::MakeSendBuffer(pkt);
	Send(SendBuffer);
}

void CServerSession::OnRecvPacket(BYTE* buffer, int32 len)
{
	CPacketSessionRef session = GetPacketSessionRef();
	PacketHeader* header = reinterpret_cast<PacketHeader*>(buffer);

	ClientPacketHandler::HandlePacket(session, buffer, len);
}

void CServerSession::OnSend(int32 len)
{
	//std::cout << "OnSend len - " << len << std::endl;
}

void CServerSession::OnMovePlayer()
{
	std::cout << "I'm Move!!!!" << std::endl;

	Protocol::C_MOVE pkt;

	Protocol::PlayerMoveInfo* info = new Protocol::PlayerMoveInfo();
	info->set_player_id(m_Id);

	Protocol::PosInfo* PosInfo = new Protocol::PosInfo();
	Protocol::Vector3* pos = new Protocol::Vector3();

	CTransform* transform = m_OwnPlayer->GetTransform();
	Vec3 playerPos = transform->GetRelativePosition();
	pos->set_x(playerPos.x);
	pos->set_y(playerPos.y);
	pos->set_z(playerPos.z);

	PosInfo->set_allocated_position(pos);
	info->set_allocated_pos_info(PosInfo);

	pkt.set_allocated_player_move_info(info);
	
	std::shared_ptr<CSendBuffer> SendBuffer = ClientPacketHandler::MakeSendBuffer(pkt);
	Send(SendBuffer);
}
