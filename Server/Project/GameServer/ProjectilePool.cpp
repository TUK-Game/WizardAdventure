#include "pch.h"
#include "ProjectilePool.h"
#include "ObjectUtil.h"
#include "BoxCollider.h"

CProjectilePoolRef g_pool = std::make_shared<CProjectilePool>();

void CProjectilePool::Init(uint32 poolNum)
{
	pool.resize(poolNum);
	used.resize(poolNum);

	for (int i = 0; i < poolNum; ++i)
	{
		pool[i] = CObjectUtil::CreateProjectile();
	}
}

CProjectileRef CProjectilePool::Allocate()
{
	for (uint32 i = 0; i < pool.size(); ++i)
	{
		if (!used[i])
		{
			m_nextProjectileId = i;
			used[i] = true;
			pool[i]->GetCollider()->SetCollisionProfile("Projectile");
			pool[i]->SetIsServerUpdate(false);
			return pool[i];
		}
	}
	return nullptr;
}

void CProjectilePool::Release(CProjectileRef obj)
{
	for (uint32 i = 0; i < pool.size(); ++i)
	{
		if (pool[i] == obj)
		{
			used[i] = false;
			//obj->GetCollider()->ClearCollisionList();
			return;
		}
	}
}



