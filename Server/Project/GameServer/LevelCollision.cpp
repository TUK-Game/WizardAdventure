#include "pch.h"
#include "LevelCollision.h"
#include "BoxCollider.h"
CLevelCollision::CLevelCollision()
{
}

CLevelCollision::~CLevelCollision()
{
}

void CLevelCollision::AddCollider(CBoxCollider* collider, ECollision_Channel channel)
{
	m_vecCollider[(int)channel].push_back(collider);
}

void CLevelCollision::Collision()
{
	size_t size = m_vecCollider[(int)ECollision_Channel::Wall].size();
	size_t size2 = m_vecCollider[(int)ECollision_Channel::Player].size();
	if (size > 1)
	{
		for (size_t i = 0; i < size; ++i)
		{
			CBoxCollider* src = m_vecCollider[(int)ECollision_Channel::Wall][i];

			for (size_t j = 0; j < size2; ++j)
			{
				CBoxCollider* dest = m_vecCollider[(int)ECollision_Channel::Player][j];

				if (src->GetOwner() == dest->GetOwner())
					continue;

				// �浹 ���������� �����ͼ� �浹���Ѿ� �ϴ� ��ü���� �Ǵ�
				const CollisionProfile* srcProfile = src->GetProfile();
				const CollisionProfile* destProfile = dest->GetProfile();

				ECollision_Interaction	srcInteraction = srcProfile->vecCollisionInteraction[(int)destProfile->channel];
				ECollision_Interaction	destInteraction = destProfile->vecCollisionInteraction[(int)srcProfile->channel];

				if (srcInteraction == ECollision_Interaction::Ignore ||
					destInteraction == ECollision_Interaction::Ignore)
					continue;

				// �浹 üũ
				if (src->Collision(dest))
				{
					// �浹 ����
					if (!src->CheckCollisionList(dest))
					{
						// ���ο��� ���� �浹ü�� �浹����Ʈ�� �߰�
						src->AddCollisionList(dest);
						dest->AddCollisionList(src);

						// �浹 ���� �Լ� ȣ��
						src->CallCollisionBegin(dest);
						dest->CallCollisionBegin(src);
					}
					else
					{
						src->CollisionEvent(dest);
						dest->CollisionEvent(src);
					}
				}

				// �浹���� ��ü �� �浹�� ������ ����
				else if (src->CheckCollisionList(dest))
				{
					// �浹����Ʈ���� ������ �浹ü ����
					src->DeleteCollisionList(dest);
					dest->DeleteCollisionList(src);

					// �浹 ���� �Լ� ȣ��
					src->CallCollisionEnd(dest);
					dest->CallCollisionEnd(src);
				}
			}
		}
	}

	m_vecCollider[(int)ECollision_Channel::Player].clear();
}
