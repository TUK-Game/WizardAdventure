#include "pch.h"
#include "ServerSession.h"
#include "ClientPacketHandler.h"
#include "Player.h"
#include "Transform.h"
#include "SkillObject.h"
#include "LevelManager.h"
#include "Level.h"
#include "Layer.h"
#include "MeshRenderer.h"
#include "Mesh.h"
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

void CServerSession::SelectMageAttribute(EPlayerAttribute attribute)
{
	Protocol::C_ENTER_GAME enterPkt;
	switch (attribute)
	{
	case EPlayerAttribute::Fire:
	{
		enterPkt.set_player_type(Protocol::PLAYER_TYPE_FIRE);
	}
	break;
	case EPlayerAttribute::Ice:
	{
		enterPkt.set_player_type(Protocol::PLAYER_TYPE_ICE);
	}
	break;
	case EPlayerAttribute::Electric:
	{
		enterPkt.set_player_type(Protocol::PLAYER_TYPE_LIGHTNING);
	}
	break;
	}
	std::shared_ptr<CSendBuffer> sendBuffer = ClientPacketHandler::MakeSendBuffer(enterPkt);
	Send(sendBuffer);
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
		
	pkt.set_ismove(true);

	std::shared_ptr<CSendBuffer> sendBuffer = ClientPacketHandler::MakeSendBuffer(pkt);
	Send(sendBuffer);

}

void CServerSession::OnActPlayer()
{
	CTransform* transform = m_OwnPlayer->GetTransform();

	Vec3 rot = transform->GetRelativeRotation();
	Vec3 dir = m_OwnPlayer->GetCurrentMoveDir();

	m_OwnPlayer->m_Amount = Vec3(0.f, 0.f, 0.f); 

	Protocol::C_MOVE pkt;
	auto* moveInfo = pkt.mutable_player_move_info();
	auto* posInfo = moveInfo->mutable_pos_info();

	moveInfo->set_player_id(m_Id);
	posInfo->set_state(m_OwnPlayer->GetStateForProtocol());
	
	ProtoToVector3(rot, posInfo->mutable_rotation());
	ProtoToVector3(dir, pkt.mutable_dir());

	pkt.set_ismove(false);
	std::shared_ptr<CSendBuffer> sendBuffer = ClientPacketHandler::MakeSendBuffer(pkt);
	Send(sendBuffer);

}

void CServerSession::SpawnSkill(CSkillObject* object)
{
	CTransform* transform = object->GetTransform();
	const Vec3& pos = transform->GetRelativePosition();
	const Vec3& scale = transform->GetRelativeScale();
	const Vec3& size = object->GetTotalMeshSize();
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
	sizeInfo->set_x(scale.x);
	sizeInfo->set_y(scale.y);
	sizeInfo->set_z(scale.z);

	pkt.mutable_size()->set_x(size.x);
	pkt.mutable_size()->set_y(size.y);
	pkt.mutable_size()->set_z(size.z);


	switch (object->GetSkillType())
	{
	case SKILL::FIRE_METEORS:
	{
		pkt.set_mesh(Protocol::FIRE_METEOR);
	}
	break;
	case SKILL::FIRE_BALL:
	{
		pkt.set_mesh(Protocol::FIRE_BALL);
	}
	break;
	case SKILL::FIRE_BALL_EXPLOSION:
	{
		pkt.set_mesh(Protocol::FIRE_BALL_EXPLOSION);
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
	
	auto& map = CLevelManager::GetInst()->GetCurrentLevel()->GetLayer(LAYER_PROJECTILE)->GetProjectileMap();
	map[object->m_ProjectileId] = object;

	std::shared_ptr<CSendBuffer> sendBuffer = ClientPacketHandler::MakeSendBuffer(pkt);
	Send(sendBuffer);
}

void CServerSession::BuyItem(uint32 itemId)
{
	Protocol::C_BUY_ITEM pkt;

	pkt.set_item_id(itemId);

	std::shared_ptr<CSendBuffer> sendBuffer = ClientPacketHandler::MakeSendBuffer(pkt);
	Send(sendBuffer);
}

void CServerSession::BuySkill(uint32 skillId)
{
	Protocol::C_BUY_SKILL pkt;

	pkt.set_skill_id(skillId);

	std::shared_ptr<CSendBuffer> sendBuffer = ClientPacketHandler::MakeSendBuffer(pkt);
	Send(sendBuffer);
}

void CServerSession::MoveSkill(CSkillObject* object)
{
	CTransform* transform = object->GetTransform();
	const Vec3& pos = transform->GetRelativePosition();
	const Vec3& scale = transform->GetRelativeScale();
	const Vec3& rot = transform->GetRelativeRotation();
	Protocol::C_MOVE_PROJECTILE pkt;
	auto* info = pkt.mutable_projectile_info();
	info->set_projectile_id(object->m_ProjectileId);

	if (object->m_bDelete)
	{
		info->set_state(Protocol::COLLISION);
	}
	else if (object->GetIsSpawnParticle())
	{
		info->set_state(Protocol::SPAWN_PARTICLE);
	}
	else
	{
		info->set_state(Protocol::MOVE_STATE);
	}

	auto* posInfo = pkt.mutable_projectile_info()->mutable_object_info()->mutable_pos_info()->mutable_position();
	auto* scaleInfo = pkt.mutable_projectile_info()->mutable_object_info()->mutable_pos_info()->mutable_size();
	auto* rotInfo = pkt.mutable_projectile_info()->mutable_object_info()->mutable_pos_info()->mutable_rotation();
	posInfo->set_x(pos.x);
	posInfo->set_y(pos.y);
	posInfo->set_z(pos.z);

	scaleInfo->set_x(scale.x);
	scaleInfo->set_y(scale.y);
	scaleInfo->set_z(scale.z);

	rotInfo->set_x(rot.x);
	rotInfo->set_y(rot.y);
	rotInfo->set_z(rot.z);

	std::shared_ptr<CSendBuffer> sendBuffer = ClientPacketHandler::MakeSendBuffer(pkt);
	Send(sendBuffer);
}
