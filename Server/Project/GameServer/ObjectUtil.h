#pragma once

class CObjectUtil
{
public:
	static CPlayerRef CreatePlayer(CGameSessionRef session);

private:
	static std::atomic<int64>	s_IdGenerator;
};

