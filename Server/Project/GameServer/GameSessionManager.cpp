#include "pch.h"
#include "GameSessionManager.h"
#include "GameSession.h"

CGameSessionManager g_SessionManager;

CGameSessionManager::CGameSessionManager()
{
	m_mapSession.insert(std::make_pair(0, nullptr));
	m_mapSession.insert(std::make_pair(1, nullptr));
	m_mapSession.insert(std::make_pair(2, nullptr));
	m_mapSession.insert(std::make_pair(3, nullptr));
}

CGameSessionManager::~CGameSessionManager()
{
}

int CGameSessionManager::Add(CGameSessionRef session)
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

	// TODO: ������� ������ ���߸� �ȵǰ� ������ �ּҸ� Disconnect ���Ѿ���.
	ASSERT_CRASH("�ο� �ʰ�");
	return -1;
}

void CGameSessionManager::Remove(CGameSessionRef session)
{
	WRITE_LOCK;

	m_mapSession[session->Id] = nullptr;
}

void CGameSessionManager::Broadcast(CSendBufferRef sendBuffer)
{
	WRITE_LOCK;
	for (auto& [id, session] : m_mapSession)
	{
		session->Send(sendBuffer);
	}
}