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
	CTransform* transform = m_OwnPlayer->GetTransform();
	Vec3 playerPos = transform->GetRelativePosition();
	Vec3 playerRotation = transform->GetRelativeRotation();
	Vec3 playerDir = m_OwnPlayer->GetTransform()->GetRelativeDir(EDir::Front);
	Vec3 amount = m_OwnPlayer->m_Amount;
	Protocol::C_MOVE pkt;
	Protocol::PlayerMoveInfo* info = new Protocol::PlayerMoveInfo();
	Protocol::Vector3* pos = new Protocol::Vector3();
	Protocol::Vector3* rot = new Protocol::Vector3();
	Protocol::Vector3* dir = new Protocol::Vector3();
	info->set_player_id(m_Id);
	pos->set_x(amount.x);
	pos->set_y(amount.y);
	pos->set_z(amount.z);
	  
	rot->set_x(playerRotation.x);
	rot->set_y(playerRotation.y);
	rot->set_z(playerRotation.z);

	dir->set_x(playerDir.x);
	dir->set_y(playerDir.y);
	dir->set_z(playerDir.z);


	pkt.mutable_player_move_info()->mutable_pos_info()->set_allocated_position(pos);
	pkt.mutable_player_move_info()->mutable_pos_info()->set_allocated_rotation(rot);
	pkt.set_allocated_dir(dir);
	pkt.mutable_player_move_info()->mutable_pos_info()->set_state(m_OwnPlayer->GetStateForProtocol());

	std::shared_ptr<CSendBuffer> SendBuffer = ClientPacketHandler::MakeSendBuffer(pkt);
	Send(SendBuffer);
}

void CServerSession::OnActPlayer()
{
	CTransform* transform = m_OwnPlayer->GetTransform();
	Vec3 playerPos = transform->GetRelativePosition();
	Vec3 playerRotation = transform->GetRelativeRotation();
	Vec3 playerDir = m_OwnPlayer->GetCurrentMoveDir();
	m_OwnPlayer->m_Amount = Vec3(0.f, 0.f, 0.f);
	Vec3 amount = m_OwnPlayer->m_Amount;

	Protocol::C_MOVE pkt;
	Protocol::PlayerMoveInfo* info = new Protocol::PlayerMoveInfo();
	Protocol::Vector3* pos = new Protocol::Vector3();
	Protocol::Vector3* rot = new Protocol::Vector3();
	Protocol::Vector3* dir = new Protocol::Vector3();
	info->set_player_id(m_Id);
	pos->set_x(amount.x);
	pos->set_y(amount.y);
	pos->set_z(amount.z);

	rot->set_x(playerRotation.x);
	rot->set_y(playerRotation.y);
	rot->set_z(playerRotation.z);

	dir->set_x(playerDir.x);
	dir->set_y(playerDir.y);
	dir->set_z(playerDir.z);
	std::cout << playerDir.x << " " << playerDir.y << " " << playerDir.z << '\n';

	pkt.mutable_player_move_info()->mutable_pos_info()->set_allocated_position(pos);
	pkt.mutable_player_move_info()->mutable_pos_info()->set_allocated_rotation(rot);
	pkt.set_allocated_dir(dir);
	pkt.mutable_player_move_info()->mutable_pos_info()->set_state(m_OwnPlayer->GetStateForProtocol());

	std::shared_ptr<CSendBuffer> SendBuffer = ClientPacketHandler::MakeSendBuffer(pkt);
	Send(SendBuffer);
}

//Protocol::C_MOVE pkt;
//
//Protocol::PlayerMoveInfo* info = new Protocol::PlayerMoveInfo();
//info->set_player_id(m_Id);
//
//Protocol::Vector3* pos = new Protocol::Vector3();
//Protocol::Vector3* rot = new Protocol::Vector3();
//
//CTransform* transform = m_OwnPlayer->GetTransform();
//Vec3 playerPos = transform->GetRelativePosition();
//Vec3 playerRotation = transform->GetRelativeRotation();
//pos->set_x(playerPos.x);
//pos->set_y(playerPos.y);
//pos->set_z(playerPos.z);
//
//rot->set_x(playerRotation.x);
//rot->set_y(playerRotation.y);
//rot->set_z(playerRotation.z);
//
//pkt.mutable_player_move_info()->mutable_pos_info()->set_allocated_position(pos);
//pkt.mutable_player_move_info()->mutable_pos_info()->set_allocated_rotation(rot);
//pkt.mutable_player_move_info()->mutable_pos_info()->set_state(m_OwnPlayer->GetStateForProtocol());
//
//std::shared_ptr<CSendBuffer> SendBuffer = ClientPacketHandler::MakeSendBuffer(pkt);
//Send(SendBuffer);