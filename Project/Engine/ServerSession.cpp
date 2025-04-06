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
	Vec3 prevPlayerPos = m_OwnPlayer->m_PrevPosition;
	Vec3 playerPos = m_OwnPlayer->m_NextPosition;;
	//Vec3 playerPos = transform->GetRelativePosition();
	Vec3 playerRotation = transform->GetRelativeRotation();

	float maxStep = 10.f;
	float totalDistanc = std::sqrt(std::pow(playerPos.x - prevPlayerPos.x, 2) + std::pow(playerPos.y - prevPlayerPos.y, 2) +
		std::pow(playerPos.z - prevPlayerPos.z, 2));
	int step = (std::max)(1, static_cast<int>(std::round(totalDistanc / maxStep)));

	Vec3 moveStep = (playerPos - prevPlayerPos);// / step;

	std::cout << "Step : " << step << " " <<  moveStep.x << " " << moveStep.y << " " << moveStep.z << std::endl;
	
	for (int i = 1; i <= 1; ++i)
	{
		Protocol::C_MOVE pkt;

		Protocol::PlayerMoveInfo* info = new Protocol::PlayerMoveInfo();
		info->set_player_id(m_Id);

		Protocol::Vector3* pos = new Protocol::Vector3();
		Protocol::Vector3* rot = new Protocol::Vector3();

		pos->set_x(prevPlayerPos.x + moveStep.x * i);
		pos->set_y(prevPlayerPos.y + moveStep.y * i);
		pos->set_z(prevPlayerPos.z + moveStep.z * i);

		rot->set_x(playerRotation.x);
		rot->set_y(playerRotation.y);
		rot->set_z(playerRotation.z);

		pkt.mutable_player_move_info()->mutable_pos_info()->set_allocated_position(pos);
		pkt.mutable_player_move_info()->mutable_pos_info()->set_allocated_rotation(rot);
		pkt.mutable_player_move_info()->mutable_pos_info()->set_state(m_OwnPlayer->GetStateForProtocol());

		std::shared_ptr<CSendBuffer> SendBuffer = ClientPacketHandler::MakeSendBuffer(pkt);
		Send(SendBuffer);
	}
	//m_OwnPlayer->GetTransform()->SetRelativePosition(playerPos);

	//m_OwnPlayer->m_PrevPosition = playerPos;
}
