#include "pch.h"
#include "Service.h"
#include "Session.h"
#include "Listener.h"

CService::CService(EServiceType type, CNetAddress address, CIocpCoreRef core, SessionFactory factory, int32 maxSessionCount)
	: m_Type(type), m_NetAddress(address), m_IocpCore(core), m_SessionFactory(factory), m_MaxSessionCount(maxSessionCount)
{

}

CService::~CService()
{
}

void CService::CloseService()
{
	// TODO
}

void CService::Broadcast(CSendBufferRef sendBuffer)
{
	WRITE_LOCK;
	for (const auto& session : m_Sessions)
	{
		session->Send(sendBuffer);
	}
}

CSessionRef CService::CreateSession()
{
	CSessionRef session = m_SessionFactory();
	session->SetService(shared_from_this());

	if (m_IocpCore->Register(session) == false)
		return nullptr;

	return session;
}

void CService::AddSession(CSessionRef session)
{
	WRITE_LOCK;
	m_SessionCount++;
	m_Sessions.insert(session);
}

void CService::ReleaseSession(CSessionRef session)
{
	WRITE_LOCK;
	ASSERT_CRASH(m_Sessions.erase(session) != 0);
	m_SessionCount--;
}



CClientService::CClientService(CNetAddress targetAddress, CIocpCoreRef core, SessionFactory factory, int32 maxSessionCount)
	: CService(EServiceType::Client, targetAddress, core, factory, maxSessionCount)
{
}

bool CClientService::Start()
{
	if (CanStart() == false)
		return false;

	const int32 sessionCount = GetMaxSessionCount();
	for (int32 i = 0; i < sessionCount; i++)
	{
		CSessionRef session = CreateSession();
		if (session->Connect() == false)
			return false;
	}

	return true;
}



CServerService::CServerService(CNetAddress address, CIocpCoreRef core, SessionFactory factory, int32 maxSessionCount)
	: CService(EServiceType::Server, address, core, factory, maxSessionCount)
{
}

bool CServerService::Start()
{
	if (CanStart() == false)
		return false;

	m_Listener = std::make_shared<CListener>();
	if (m_Listener == nullptr)
		return false;

	CServerServiceRef service = static_pointer_cast<CServerService>(shared_from_this());
	if (m_Listener->StartAccept(service) == false)
		return false;

	return true;
}

void CServerService::CloseService()
{
	// TODO

	CService::CloseService();
}
