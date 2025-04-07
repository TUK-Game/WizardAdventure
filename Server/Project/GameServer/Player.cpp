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
	std::cout << "나 박음" << std::endl;
	//XMFLOAT3 dc = dest->GetBoundingBox().Center;
	//XMFLOAT3 de = dest->GetBoundingBox().Extents;

	//XMFLOAT3 sc = src->GetBoundingBox().Center;
	//XMFLOAT3 se = src->GetBoundingBox().Extents;

	//Protocol::Vector3* pos = new Protocol::Vector3;
	//pos->CopyFrom(PlayerInfo->object_info().pos_info().position());
	//std::cout << "Before Setting : " << pos->x() << " " << pos->y() << " " << pos->z() << '\n';
	//// min/max 좌표 계산
	//XMFLOAT3 srcMin = { sc.x - se.x, sc.y - se.y, sc.z - se.z };
	//XMFLOAT3 srcMax = { sc.x + se.x, sc.y + se.y, sc.z + se.z };

	//XMFLOAT3 destMin = { dc.x - de.x, dc.y - de.y, dc.z - de.z };
	//XMFLOAT3 destMax = { dc.x + de.x, dc.y + de.y, dc.z + de.z };

	//// AABB 충돌 판정 (XZ 축만)
	//bool isColliding =
	//	(srcMin.x <= destMax.x && srcMax.x >= destMin.x) &&
	//	(srcMin.z <= destMax.z && srcMax.z >= destMin.z);

	//if (!isColliding)
	//	return; // 충돌 없으면 리턴

	//// 겹친 거리 계산
	//float overlapX = (std::min)(srcMax.x, destMax.x) - (std::max)(srcMin.x, destMin.x);
	//float overlapZ = (std::min)(srcMax.z, destMax.z) - (std::max)(srcMin.z, destMin.z);

	//const float margin = 30.05f; // 벽에서 살짝 떨어지게

	//if (overlapX < overlapZ)
	//{
	//	// X축 기준 보정
	//	if (sc.x < dc.x)
	//		pos->set_x(destMin.x - se.x - margin); // 왼쪽 충돌
	//	else
	//		pos->set_x(destMax.x + se.x + margin); // 오른쪽 충돌
	//	// Z축은 유지
	//}
	//else
	//{
	//	// Z축 기준 보정
	//	if (sc.z < dc.z)
	//		pos->set_z(destMin.z - se.z - margin); // 앞쪽 충돌
	//	else
	//		pos->set_z(destMax.z + se.z + margin); // 뒤쪽 충돌
	//	// X축은 유지
	//}
	//std::cout << "Setting : " << pos->x() << " " << pos->y() << " " << pos->z() << '\n';
	//PlayerInfo->mutable_object_info()->mutable_pos_info()->set_allocated_position(pos);
	//CPlayerRef player = m_Session.lock()->Player.load();
	//g_Room->DoAsync(&CRoom::HandleMovePlayer, player);
}

void CPlayer::CollisionEvent(CBoxCollider* src, CBoxCollider* dest)
{
}
