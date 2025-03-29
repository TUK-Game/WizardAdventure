#pragma once

class CGameSession;	// Client Session

using CGameSessionRef = std::shared_ptr<CGameSession>;

// TODO: Singleton
class CGameSessionManager
{
public:
	CGameSessionManager();
	~CGameSessionManager();

public:
	int Add(CGameSessionRef session);
	void Remove(CGameSessionRef session);
	void Broadcast(CSendBufferRef sendBuffer);	// 모든 세션에게 Broadcast

private:
	USE_LOCK;
	std::unordered_map<int, std::shared_ptr<CGameSession>>	m_mapSession;
};

// TEMP
extern CGameSessionManager g_SessionManager;
