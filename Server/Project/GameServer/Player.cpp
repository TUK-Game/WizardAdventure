#include "pch.h"
#include "Player.h"
#include "CreatureCollider.h"
#include "Room.h"

CPlayer::CPlayer()
{
	m_bPlayer = true;
	ObjectInfo = new Protocol::ObjectInfo();
	PosInfo = new Protocol::PosInfo();
	PlayerInfo = new Protocol::PlayerInfo();
	ObjectInfo->set_allocated_pos_info(PosInfo);
	PlayerInfo->set_allocated_object_info(ObjectInfo);

	m_BoxCollider = new CCreatureCollider();
	m_BoxCollider->SetOwner(this);
}

CPlayer::~CPlayer()
{
}

void CPlayer::Update()
{
	CGameObject::Update();
}

void CPlayer::CollisionBegin(CBoxCollider* src, CBoxCollider* dest)
{
	std::cout << "�� ����" << std::endl;
	//XMFLOAT3 dc = dest->GetBoundingBox().Center;
	//XMFLOAT3 de = dest->GetBoundingBox().Extents;

	//XMFLOAT3 sc = src->GetBoundingBox().Center;
	//XMFLOAT3 se = src->GetBoundingBox().Extents;

	//Protocol::Vector3* pos = new Protocol::Vector3;
	//pos->CopyFrom(PlayerInfo->object_info().pos_info().position());
	//std::cout << "Before Setting : " << pos->x() << " " << pos->y() << " " << pos->z() << '\n';
	//// min/max ��ǥ ���
	//XMFLOAT3 srcMin = { sc.x - se.x, sc.y - se.y, sc.z - se.z };
	//XMFLOAT3 srcMax = { sc.x + se.x, sc.y + se.y, sc.z + se.z };

	//XMFLOAT3 destMin = { dc.x - de.x, dc.y - de.y, dc.z - de.z };
	//XMFLOAT3 destMax = { dc.x + de.x, dc.y + de.y, dc.z + de.z };

	//// AABB �浹 ���� (XZ �ุ)
	//bool isColliding =
	//	(srcMin.x <= destMax.x && srcMax.x >= destMin.x) &&
	//	(srcMin.z <= destMax.z && srcMax.z >= destMin.z);

	//if (!isColliding)
	//	return; // �浹 ������ ����

	//// ��ģ �Ÿ� ���
	//float overlapX = (std::min)(srcMax.x, destMax.x) - (std::max)(srcMin.x, destMin.x);
	//float overlapZ = (std::min)(srcMax.z, destMax.z) - (std::max)(srcMin.z, destMin.z);

	//const float margin = 30.05f; // ������ ��¦ ��������

	//if (overlapX < overlapZ)
	//{
	//	// X�� ���� ����
	//	if (sc.x < dc.x)
	//		pos->set_x(destMin.x - se.x - margin); // ���� �浹
	//	else
	//		pos->set_x(destMax.x + se.x + margin); // ������ �浹
	//	// Z���� ����
	//}
	//else
	//{
	//	// Z�� ���� ����
	//	if (sc.z < dc.z)
	//		pos->set_z(destMin.z - se.z - margin); // ���� �浹
	//	else
	//		pos->set_z(destMax.z + se.z + margin); // ���� �浹
	//	// X���� ����
	//}
	//std::cout << "Setting : " << pos->x() << " " << pos->y() << " " << pos->z() << '\n';
	//PlayerInfo->mutable_object_info()->mutable_pos_info()->set_allocated_position(pos);
	//CPlayerRef player = m_Session.lock()->Player.load();
	//g_Room->DoAsync(&CRoom::HandleMovePlayer, player);
}

void CPlayer::CollisionEvent(CBoxCollider* src, CBoxCollider* dest)
{
}
