#pragma once

class CObjectUtil
{
public:
	static CPlayerRef CreatePlayer(CGameSessionRef session);
	static CGameObjectRef CreateObject();
	static CMonsterRef CreateMonster();
	static CProjectileRef CreateProjectile();

private:
	static std::atomic<int64>	s_IdGenerator;
};

