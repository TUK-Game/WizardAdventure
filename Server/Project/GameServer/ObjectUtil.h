#pragma once

class CObjectUtil
{
public:
	static CPlayerRef CreatePlayer(CGameSessionRef session);
	static CGameObjectRef CreateObject();

private:
	static std::atomic<int64>	s_IdGenerator;
};

