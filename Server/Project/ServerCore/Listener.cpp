#include "pch.h"
#include "Listener.h"
#include "SocketUtils.h"
#include "IocpEvent.h"
#include "Session.h"
#include "Service.h"

CListener::~CListener()
{
	CSocketUtils::Close(m_Socket);

	for (CAcceptEvent* acceptEvent : m_vecAcceptEvent)
	{
		// TODO

		delete(acceptEvent);
	}
}

bool CListener::StartAccept(CServerServiceRef service)
{
	m_Service = service;
	if (m_Service == nullptr)
		return false;

	m_Socket = CSocketUtils::CreateSocket();
	if (m_Socket == INVALID_SOCKET)
		return false;

	if (m_Service->GetIocpCore()->Register(shared_from_this()) == false)
		return false;

	if (CSocketUtils::SetReuseAddress(m_Socket, true) == false)
		return false;

	if (CSocketUtils::SetLinger(m_Socket, 0, 0) == false)
		return false;

	if (CSocketUtils::Bind(m_Socket, m_Service->GetNetAddress()) == false)
		return false;

	if (CSocketUtils::Listen(m_Socket) == false)
		return false;

	const int32 acceptCount = m_Service->GetMaxSessionCount();
	for (int32 i = 0; i < acceptCount; i++)
	{
		CAcceptEvent* acceptEvent = new CAcceptEvent();
		acceptEvent->m_Owner = shared_from_this();
		m_vecAcceptEvent.push_back(acceptEvent);
		RegisterAccept(acceptEvent);
	}

	return true;
}

void CListener::CloseSocket()
{
	CSocketUtils::Close(m_Socket);
}

HANDLE CListener::GetHandle()
{
	return reinterpret_cast<HANDLE>(m_Socket);
}

void CListener::Dispatch(CIocpEvent* iocpEvent, int32 numOfBytes)
{
	ASSERT_CRASH(iocpEvent->m_EventType == EEventType::Accept);
	CAcceptEvent* acceptEvent = static_cast<CAcceptEvent*>(iocpEvent);
	ProcessAccept(acceptEvent);
}

void CListener::RegisterAccept(CAcceptEvent* acceptEvent)
{
	CSessionRef session = m_Service->CreateSession(); // Register IOCP

	acceptEvent->Init();
	acceptEvent->m_Session = session;

	DWORD bytesReceived = 0;
	if (false == CSocketUtils::s_AcceptEx(m_Socket, session->GetSocket(), session->m_RecvBuffer.WritePos(), 0, sizeof(SOCKADDR_IN) + 16, sizeof(SOCKADDR_IN) + 16, OUT & bytesReceived, static_cast<LPOVERLAPPED>(acceptEvent)))
	{
		const int32 errorCode = ::WSAGetLastError();
		if (errorCode != WSA_IO_PENDING)
		{
			// 일단 다시 Accept 걸어준다
			RegisterAccept(acceptEvent);
		}
	}
}

void CListener::ProcessAccept(CAcceptEvent* acceptEvent)
{
	CSessionRef session = acceptEvent->m_Session;

	if (false == CSocketUtils::SetUpdateAcceptSocket(session->GetSocket(), m_Socket))
	{
		RegisterAccept(acceptEvent);
		return;
	}

	SOCKADDR_IN sockAddress;
	int32 sizeOfSockAddr = sizeof(sockAddress);
	if (SOCKET_ERROR == ::getpeername(session->GetSocket(), OUT reinterpret_cast<SOCKADDR*>(&sockAddress), &sizeOfSockAddr))
	{
		RegisterAccept(acceptEvent);
		return;
	}

	session->SetNetAddress(CNetAddress(sockAddress));
	session->ProcessConnect();
	RegisterAccept(acceptEvent);
}