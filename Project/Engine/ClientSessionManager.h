#pragma once

class CClientSession;

class CClientSessionManager :
	public CSingleton<CClientSessionManager>
{
	DECLARE_SINGLETON(CClientSessionManager)

public:
	int Add(std::shared_ptr<CClientSession> session);
	void Remove(std::shared_ptr<CClientSession> session);
	void Broadcast(std::shared_ptr<CSendBuffer> sendBuffer);

private:
	USE_LOCK;
	std::unordered_map<int, std::shared_ptr<CClientSession>>	m_mapSession;
};



