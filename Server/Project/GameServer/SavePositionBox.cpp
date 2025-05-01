#include "pch.h"
#include "SavePositionBox.h"
#include "BoxCollider.h"
#include "Player.h"

CSavePositionBox::CSavePositionBox()
{
}

CSavePositionBox::~CSavePositionBox()
{
}

void CSavePositionBox::CollisionBegin(CBoxCollider* src, CBoxCollider* dest)
{
	CPlayer* player = dynamic_cast<CPlayer*>(dest->GetOwner());
	if (player)
	{
		const auto& pos = ObjectInfo->pos_info().position();
		player->SetSafePosition(Vec3(pos.x(), pos.y(), pos.z()));
		std::cout << "플레이어의 위치를 얻다\n";
	}
}
