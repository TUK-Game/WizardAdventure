#include "pch.h"
#include "LevelCollision.h"
#include "BoxCollider.h"
#include "GameObject.h"

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
    for (int i = 0; i < (int)ECollision_Channel::Max; ++i)
    {
        size_t srcSize = m_vecCollider[i].size();
        if (srcSize == 0 || (int)ECollision_Channel::Wall == i)
            continue;

        for (int j = 0; j < (int)ECollision_Channel::Max; ++j)
        {
            size_t destSize = m_vecCollider[j].size();
            if (destSize == 0 || (int)ECollision_Channel::Wall == j)
                continue;

            for (size_t s = 0; s < srcSize; ++s)
            {
                CBoxCollider* src = m_vecCollider[i][s];
                const CollisionProfile* srcProfile = src->GetProfile();

                size_t start = (i == j) ? s + 1 : 0;
                for (size_t d = start; d < destSize; ++d)
                {
                    CBoxCollider* dest = m_vecCollider[j][d];
                    if (src == dest || src->GetOwner() == dest->GetOwner())
                        continue;

                    const CollisionProfile* destProfile = dest->GetProfile();

                    ECollision_Interaction srcInteraction = srcProfile->vecCollisionInteraction[(int)destProfile->channel];
                    ECollision_Interaction destInteraction = destProfile->vecCollisionInteraction[(int)srcProfile->channel];

                    if (srcInteraction == ECollision_Interaction::Ignore || destInteraction == ECollision_Interaction::Ignore)
                        continue;

                    if (src->Collision(dest))
                    {
                        if (!src->CheckCollisionList(dest))
                        {
                            src->AddCollisionList(dest);
                            dest->AddCollisionList(src);

                            src->CallCollisionBegin(dest);
                            dest->CallCollisionBegin(src);
                        }
                        else
                        {
                            src->CollisionEvent(dest);
                            dest->CollisionEvent(src);
                        }
                    }
                    else if (src->CheckCollisionList(dest))
                    {
                        src->DeleteCollisionList(dest);
                        dest->DeleteCollisionList(src);

                        src->CallCollisionEnd(dest);
                        dest->CallCollisionEnd(src);
                    }
                }
            }
        }
    }

    for (int i = 0; i < (int)ECollision_Channel::Max; ++i)
    {
        if ((int)ECollision_Channel::Wall == i)
            continue;

        m_vecCollider[i].clear();
    }
}

bool CLevelCollision::CollisionWithWall(CBoxCollider* collider)
{
	size_t size = m_vecCollider[(int)ECollision_Channel::Wall].size();
	if (size > 1)
	{
		CBoxCollider* dest = collider;
		for (size_t i = 0; i < size; ++i)
		{
			CBoxCollider* src = m_vecCollider[(int)ECollision_Channel::Wall][i];

			// 충돌 체크
			if (src->Collision(dest))
			{
				src->CallCollisionBegin(dest);
				dest->CallCollisionBegin(src);
				return true;
			}
		}
	}
	return false;
}
