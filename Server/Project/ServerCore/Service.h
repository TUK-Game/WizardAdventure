#pragma once

#include "NetAddress.h"
#include "IocpCore.h"
#include "Listener.h"

enum class EServiceType : uint8
{
	Server,
	Client
};

using SessionFactory = std::function<CSessionRef(void)>;

class CService : public std::enable_shared_from_this<CService>
{
public:
	CService(EServiceType type, CNetAddress address, CIocpCoreRef core, SessionFactory factory, int32 maxSessionCount = 1);
	virtual ~CService();

	virtual bool		Start() = 0;
	bool				CanStart() { return m_SessionFactory != nullptr; }

	virtual void		CloseService();
	void				SetSessionFactory(SessionFactory func) { m_SessionFactory = func; }

	void				Broadcast(CSendBufferRef sendBuffer);
	CSessionRef			CreateSession();
	void				AddSession(CSessionRef session);
	void				ReleaseSession(CSessionRef session);
	int32				GetCurrentSessionCount() { return m_SessionCount; }
	int32				GetMaxSessionCount() { return m_MaxSessionCount; }

public:
	EServiceType		GetServiceType() { return m_Type; }
	CNetAddress			GetNetAddress() { return m_NetAddress; }
	CIocpCoreRef&		GetIocpCore() { return m_IocpCore; }

protected:
	USE_LOCK;
	EServiceType			m_Type;
	CNetAddress				m_NetAddress = {};
	CIocpCoreRef			m_IocpCore;

	std::set<CSessionRef>	m_Sessions;
	int32					m_SessionCount = 0;
	int32					m_MaxSessionCount = 0;
	SessionFactory			m_SessionFactory;
};



class CClientService : public CService
{
public:
	CClientService(CNetAddress targetAddress, CIocpCoreRef core, SessionFactory factory, int32 maxSessionCount = 1);
	virtual ~CClientService() {}

	virtual bool	Start() override;
};



class CServerService : public CService
{
public:
	CServerService(CNetAddress targetAddress, CIocpCoreRef core, SessionFactory factory, int32 maxSessionCount = 1);
	virtual ~CServerService() {}

	virtual bool	Start() override;
	virtual void	CloseService() override;

private:
	CListenerRef		m_Listener = nullptr;
};