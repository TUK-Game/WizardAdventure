#pragma once
#include "Projectile.h"

class CProjectilePool
{
public:
	void Init(uint32 poolNum);
	CProjectileRef Allocate();
	void Release(CProjectileRef obj);

	uint32 GetIdx() { return m_nextProjectileId; }
private:
	std::vector<CProjectileRef> pool;
	std::vector<bool> used;

	uint32 m_nextProjectileId{};
};

extern CProjectilePoolRef g_pool;
