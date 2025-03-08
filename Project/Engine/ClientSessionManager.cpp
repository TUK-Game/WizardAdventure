#include "pch.h"
#include "ClientSessionManager.h"
#include "ClientSession.h"

CClientSessionManager::CClientSessionManager()
{
	m_mapSession.insert(std::make_pair(2, nullptr));
	m_mapSession.insert(std::make_pair(3, nullptr));
	m_mapSession.insert(std::make_pair(4, nullptr));
}

CClientSessionManager::~CClientSessionManager()
{
}

int CClientSessionManager::Add(std::shared_ptr<CClientSession> session)
{
	WRITE_LOCK;

	for (auto& [id, clientSession] : m_mapSession)
	{
		if (!clientSession)
		{
			clientSession = session;
			return id;
		}
	}

	ASSERT_CRASH("인원 초과");
	return -1;
}

void CClientSessionManager::Remove(std::shared_ptr<CClientSession> session)
{
	WRITE_LOCK;

	m_mapSession[session->m_Id] = nullptr;
}

void CClientSessionManager::Broadcast(std::shared_ptr<CSendBuffer> sendBuffer)
{
	WRITE_LOCK;
	for (auto& [id, session] : m_mapSession)
	{
		session->Send(sendBuffer);
	}
}
