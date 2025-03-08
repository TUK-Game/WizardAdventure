#pragma once

class CSession;

enum class EEventType : uint8
{
	Connect,
	Disconnect,
	Accept,
	//PreRecv,
	Recv,
	Send
};

class CIocpEvent : public OVERLAPPED
{
public:
	CIocpEvent(EEventType type);

	void			Init();

public:
	EEventType		m_EventType;
	CIocpObjectRef	m_Owner;
};



class CConnectEvent : public CIocpEvent
{
public:
	CConnectEvent() : CIocpEvent(EEventType::Connect) { }
};



class CDisconnectEvent : public CIocpEvent
{
public:
	CDisconnectEvent() : CIocpEvent(EEventType::Disconnect) { }
};



class CAcceptEvent : public CIocpEvent
{
public:
	CAcceptEvent() : CIocpEvent(EEventType::Accept) { }

public:
	CSessionRef	m_Session = nullptr;
};



class CRecvEvent : public CIocpEvent
{
public:
	CRecvEvent() : CIocpEvent(EEventType::Recv) { }
};



class CSendEvent : public CIocpEvent
{
public:
	CSendEvent() : CIocpEvent(EEventType::Send) { }
	 
	std::vector<CSendBufferRef> m_vecSendBuffer;
};