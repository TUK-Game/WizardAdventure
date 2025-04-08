#include "pch.h"
#include "LevelCollision.h"
#include "BaseCollider.h"
CLevelCollision::CLevelCollision()
{
	m_vecCollider.reserve(100);
}

CLevelCollision::~CLevelCollision()
{
}

void CLevelCollision::AddCollider(CBaseCollider* collider)
{
	//m_vecCollider.push_back(collider);
}

void CLevelCollision::Collision()
{
	//size_t size = m_vecCollider.size();
	//if (size > 1)
	//{
	//	for (size_t i = 0; i < size - 1; ++i)
	//	{
	//		CBaseCollider* src = m_vecCollider[i];

	//		for (size_t j = i + 1; j < size; ++j)
	//		{
	//			CBaseCollider* dest = m_vecCollider[j];

	//			if (src->GetOwner() == dest->GetOwner())
	//				continue;

	//			// 충돌 프로파일을 가져와서 충돌시켜야 하는 물체인지 판단
	//			const CollisionProfile* srcProfile = src->GetProfile();
	//			const CollisionProfile* destProfile = dest->GetProfile();

	//			ECollision_Interaction	srcInteraction = srcProfile->vecCollisionInteraction[(int)destProfile->channel];
	//			ECollision_Interaction	destInteraction = destProfile->vecCollisionInteraction[(int)srcProfile->channel];

	//			if (srcInteraction == ECollision_Interaction::Ignore ||
	//				destInteraction == ECollision_Interaction::Ignore)
	//				continue;

	//			// 충돌 체크
	//			if (src->Collision(dest))
	//			{
	//				// 충돌 순간
	//				if (!src->CheckCollisionList(dest))
	//				{
	//					// 서로에게 상대방 충돌체를 충돌리스트에 추가
	//					src->AddCollisionList(dest);
	//					dest->AddCollisionList(src);

	//					// 충돌 시작 함수 호출
	//					src->CallCollisionBegin(dest);
	//					dest->CallCollisionBegin(src);
	//				}
	//			}

	//			// 충돌중인 물체 간 충돌이 끝나는 순간
	//			else if (src->CheckCollisionList(dest))
	//			{
	//				// 충돌리스트에서 서로의 충돌체 제거
	//				src->DeleteCollisionList(dest);
	//				dest->DeleteCollisionList(src);

	//				// 충돌 종료 함수 호출
	//				src->CallCollisionEnd(dest);
	//				dest->CallCollisionEnd(src);
	//			}
	//		}
	//	}
	//}

	//m_vecCollider.clear();
}
