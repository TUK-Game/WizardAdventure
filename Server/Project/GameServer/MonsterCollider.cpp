#include "pch.h"
#include "MonsterCollider.h"
#include "GameObject.h"
#include "Room.h"
#include "LevelCollision.h"
#include "Monster.h"

void CMonsterCollider::Update()
{
	Protocol::Vector3 pos = ((CMonster*)m_Owner)->MonsterInfo->object_info().pos_info().position();
	m_BoundingBox.Center = XMFLOAT3(pos.x() + m_Offset.x, pos.y() + m_Offset.y, pos.z() + m_Offset.z);
	// temp
	g_Room->GetLevelCollision()->AddCollider(this, m_Channel);
}

void CMonsterCollider::SetBoxInfo(XMFLOAT3 centerPos, XMFLOAT3 size, XMFLOAT3 offset)
{
	m_BoundingBox.Center = centerPos;

	m_BoundingBox.Extents = XMFLOAT3(size.x / 2, size.y / 2, size.z / 2);
	m_Offset = offset;
	((CMonster*)m_Owner)->MonsterInfo->mutable_object_info()->mutable_pos_info()->mutable_position()->set_x(centerPos.x);
	((CMonster*)m_Owner)->MonsterInfo->mutable_object_info()->mutable_pos_info()->mutable_position()->set_y(centerPos.y);
	((CMonster*)m_Owner)->MonsterInfo->mutable_object_info()->mutable_pos_info()->mutable_position()->set_z(centerPos.z);
}
