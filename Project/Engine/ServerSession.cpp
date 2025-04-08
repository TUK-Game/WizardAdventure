#include "pch.h"
#include "ServerSession.h"
#include "ClientPacketHandler.h"
#include "Player.h"
#include "Transform.h"

void ProtoToVector3(const Vec3& from, Protocol::Vector3* to)
{
	to->set_x(from.x);
	to->set_y(from.y);
	to->set_z(from.z);
}

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
	CTransform* transform = m_OwnPlayer->GetTransform();
	Vec3 pos = m_OwnPlayer->m_Amount;
	Vec3 rot = transform->GetRelativeRotation();
	Vec3 dir = transform->GetRelativeDir(EDir::Front);

	Protocol::C_MOVE pkt;
	auto* moveInfo = pkt.mutable_player_move_info();
	auto* posInfo = moveInfo->mutable_pos_info();

	moveInfo->set_player_id(m_Id);
	posInfo->set_state(m_OwnPlayer->GetStateForProtocol());

	ProtoToVector3(pos, posInfo->mutable_position());
	ProtoToVector3(rot, posInfo->mutable_rotation());
	ProtoToVector3(dir, pkt.mutable_dir());

	std::shared_ptr<CSendBuffer> sendBuffer = ClientPacketHandler::MakeSendBuffer(pkt);
	Send(sendBuffer);

}

void CServerSession::OnActPlayer()
{
	CTransform* transform = m_OwnPlayer->GetTransform();

	Vec3 pos = Vec3(0.f, 0.f, 0.f); 
	Vec3 rot = transform->GetRelativeRotation();
	Vec3 dir = m_OwnPlayer->GetCurrentMoveDir();

	m_OwnPlayer->m_Amount = pos; 

	Protocol::C_MOVE pkt;
	auto* moveInfo = pkt.mutable_player_move_info();
	auto* posInfo = moveInfo->mutable_pos_info();

	moveInfo->set_player_id(m_Id);
	posInfo->set_state(m_OwnPlayer->GetStateForProtocol());
	
	ProtoToVector3(pos, posInfo->mutable_position());
	ProtoToVector3(rot, posInfo->mutable_rotation());
	ProtoToVector3(dir, pkt.mutable_dir());

	std::cout << dir.x << " " << dir.y << " " << dir.z << '\n';

	std::shared_ptr<CSendBuffer> sendBuffer = ClientPacketHandler::MakeSendBuffer(pkt);
	Send(sendBuffer);

}

	