#include "pch.h"
#include "ServerSession.h"
#include "ClientPacketHandler.h"
#include "Player.h"
#include "Transform.h"
#include "SkillObject.h"
#include "LevelManager.h"
#include "Level.h"
#include "Layer.h"

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

	std::shared_ptr<CSendBuffer> sendBuffer = ClientPacketHandler::MakeSendBuffer(pkt);
	Send(sendBuffer);

}

void CServerSession::SpawnSkill(CSkillObject* object)
{
	CTransform* transform = object->GetTransform();
	const Vec3& pos = transform->GetRelativePosition();
	const Vec3& size = transform->GetRelativeScale();
	Protocol::C_SPAWN_PROJECTILE pkt;
	auto* info = pkt.mutable_info();

	object->m_ProjectileId = m_projectileId;
	info->set_new_projectile_id(m_projectileId++);
	info->set_player_id(m_Id);
	info->mutable_dir()->set_x(1);
	info->set_bcollisionexplosion(object->GetCollisionExplosion());

	auto* posInfo = pkt.mutable_info()->mutable_spawn_pos();
	posInfo->set_x(pos.x);
	posInfo->set_y(pos.y);
	posInfo->set_z(pos.z);

	auto* sizeInfo = pkt.mutable_info()->mutable_size();
	sizeInfo->set_x(size.x);
	sizeInfo->set_y(size.y);
	sizeInfo->set_z(size.z);

	switch (object->GetSkillType())
	{
	case SKILL::FIRE_BALL:
	case SKILL::FIRE_METEORS:
	{
		pkt.set_mesh(Protocol::FIRE_BALL);
	}
	break;
	case SKILL::FIRE_CIRCLE:
	{
		pkt.set_mesh(Protocol::FIRE_CIRCLE);
	}
	break;
	case SKILL::FIRE_PILLAR:
	{
		pkt.set_mesh(Protocol::FIRE_PILLAR);
	}
	break;
	case SKILL::FIRE_SWORD:
	{
		pkt.set_mesh(Protocol::FIRE_SWORD);
	}
	break;
	}

	pkt.mutable_info()->set_damage(object->GetDamage());
	
	auto& map = CLevelManager::GetInst()->GetCurrentLevel()->GetLayer(12)->GetProjectileMap();
	//if (map.find(object->m_ProjectileId) != map.end())
	//{
	//	map.erase(object->m_ProjectileId);
	//}
	map[object->m_ProjectileId] = object;

	std::shared_ptr<CSendBuffer> sendBuffer = ClientPacketHandler::MakeSendBuffer(pkt);
	Send(sendBuffer);
}

void CServerSession::MoveSkill(CSkillObject* object)
{
	CTransform* transform = object->GetTransform();
	const Vec3& pos = transform->GetRelativePosition();
	const Vec3& rot = transform->GetRelativeRotation();
	Protocol::C_MOVE_PROJECTILE pkt;
	auto* info = pkt.mutable_projectile_info();
	info->set_projectile_id(object->m_ProjectileId);

	if (object->m_bDelete)
	{
		info->set_state(Protocol::COLLISION);
	}
	else
		info->set_state(Protocol::MOVE_STATE);

	auto* posInfo = pkt.mutable_projectile_info()->mutable_object_info()->mutable_pos_info()->mutable_position();
	auto* rotInfo = pkt.mutable_projectile_info()->mutable_object_info()->mutable_pos_info()->mutable_rotation();
	posInfo->set_x(pos.x);
	posInfo->set_y(pos.y);
	posInfo->set_z(pos.z);

	rotInfo->set_x(rot.x);
	rotInfo->set_y(rot.y);
	rotInfo->set_z(rot.z);

	std::shared_ptr<CSendBuffer> sendBuffer = ClientPacketHandler::MakeSendBuffer(pkt);
	Send(sendBuffer);
}
