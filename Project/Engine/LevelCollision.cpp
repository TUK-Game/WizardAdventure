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

	//			// �浹 ���������� �����ͼ� �浹���Ѿ� �ϴ� ��ü���� �Ǵ�
	//			const CollisionProfile* srcProfile = src->GetProfile();
	//			const CollisionProfile* destProfile = dest->GetProfile();

	//			ECollision_Interaction	srcInteraction = srcProfile->vecCollisionInteraction[(int)destProfile->channel];
	//			ECollision_Interaction	destInteraction = destProfile->vecCollisionInteraction[(int)srcProfile->channel];

	//			if (srcInteraction == ECollision_Interaction::Ignore ||
	//				destInteraction == ECollision_Interaction::Ignore)
	//				continue;

	//			// �浹 üũ
	//			if (src->Collision(dest))
	//			{
	//				// �浹 ����
	//				if (!src->CheckCollisionList(dest))
	//				{
	//					// ���ο��� ���� �浹ü�� �浹����Ʈ�� �߰�
	//					src->AddCollisionList(dest);
	//					dest->AddCollisionList(src);

	//					// �浹 ���� �Լ� ȣ��
	//					src->CallCollisionBegin(dest);
	//					dest->CallCollisionBegin(src);
	//				}
	//			}

	//			// �浹���� ��ü �� �浹�� ������ ����
	//			else if (src->CheckCollisionList(dest))
	//			{
	//				// �浹����Ʈ���� ������ �浹ü ����
	//				src->DeleteCollisionList(dest);
	//				dest->DeleteCollisionList(src);

	//				// �浹 ���� �Լ� ȣ��
	//				src->CallCollisionEnd(dest);
	//				dest->CallCollisionEnd(src);
	//			}
	//		}
	//	}
	//}

	//m_vecCollider.clear();
}
