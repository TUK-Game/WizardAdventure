#include "pch.h"
#include "Session.h"
#include "SocketUtils.h"
#include "Service.h"

CSession::CSession() : m_RecvBuffer(BUFFER_SIZE)
{
	m_Socket = CSocketUtils::CreateSocket();
}

CSession::~CSession()
{
	CSocketUtils::Close(m_Socket);
}

void CSession::Send(CSendBufferRef sendBuffer)
{
	if (IsConnected() == false)
		return;

	bool registerSend = false;

	// ���� RegisterSend�� �ɸ��� ���� ���¶��, �ɾ��ش�
	{
		WRITE_LOCK;

		m_SendQueue.push(sendBuffer);

		if (m_bSendRegistered.exchange(true) == false)
			registerSend = true;

		if (registerSend)
			RegisterSend();
	}
}

bool CSession::Connect()
{
	return RegisterConnect();
}

void CSession::Disconnect(const WCHAR* cause)
{
	if (m_bConnected.exchange(false) == false)
		return;

	// TEMP
	std::wcout << "Disconnect : " << cause << std::endl;

	RegisterDisconnect();
}

HANDLE CSession::GetHandle()
{
	return reinterpret_cast<HANDLE>(m_Socket);
}

void CSession::Dispatch(CIocpEvent* iocpEvent, int32 numOfBytes)
{
	switch (iocpEvent->m_EventType)
	{
	case EEventType::Connect:
		ProcessConnect();
		break;
	case EEventType::Disconnect:
		ProcessDisconnect();
		break;
	case EEventType::Recv:
		ProcessRecv(numOfBytes);
		break;
	case EEventType::Send:
		ProcessSend(numOfBytes);
		break;
	default:
		break;
	}
}

bool CSession::RegisterConnect()
{
	if (IsConnected())
		return false;

	if (GetService()->GetServiceType() != EServiceType::Client)
		return false;

	if (CSocketUtils::SetReuseAddress(m_Socket, true) == false)
		return false;

	if (CSocketUtils::BindAnyAddress(m_Socket, 0) == false)
		return false;

	m_ConnectEvent.Init();
	m_ConnectEvent.m_Owner = shared_from_this(); // ADD_REF

	DWORD numOfBytes = 0;
	SOCKADDR_IN sockAddr = GetService()->GetNetAddress().GetSockAddr();
	if (false == CSocketUtils::s_ConnectEx(m_Socket, reinterpret_cast<SOCKADDR*>(&sockAddr), sizeof(sockAddr), nullptr, 0, &numOfBytes, &m_ConnectEvent))
	{
		int32 errorCode = ::WSAGetLastError();
		if (errorCode != WSA_IO_PENDING)
		{
			m_ConnectEvent.m_Owner = nullptr; // RELEASE_REF
			return false;
		}
	}

	return true;
}

bool CSession::RegisterDisconnect()
{
	m_DisconnectEvent.Init();
	m_DisconnectEvent.m_Owner = shared_from_this(); // ADD_REF

	if (false == CSocketUtils::s_DisconnectEx(m_Socket, &m_DisconnectEvent, TF_REUSE_SOCKET, 0))
	{
		int32 errorCode = ::WSAGetLastError();
		if (errorCode != WSA_IO_PENDING)
		{
			m_DisconnectEvent.m_Owner = nullptr; // RELEASE_REF
			return false;
		}
	}

	return true;
}

void CSession::RegisterRecv()
{
	if (IsConnected() == false)
		return;

	m_RecvEvent.Init();
	m_RecvEvent.m_Owner = shared_from_this(); // ADD_REF

	WSABUF wsaBuf;
	wsaBuf.buf = reinterpret_cast<char*>(m_RecvBuffer.WritePos());
	wsaBuf.len = m_RecvBuffer.FreeSize();

	DWORD numOfBytes = 0;
	DWORD flags = 0;
	if (SOCKET_ERROR == ::WSARecv(m_Socket, &wsaBuf, 1, OUT &numOfBytes, OUT &flags, &m_RecvEvent, nullptr))
	{
		int32 errorCode = ::WSAGetLastError();
		if (errorCode != WSA_IO_PENDING)
		{
			HandleError(errorCode);
			m_RecvEvent.m_Owner = nullptr; // RELEASE_REF
		}
	}
}

void CSession::RegisterSend()
{
	if (IsConnected() == false)
		return;

	m_SendEvent.Init();
	m_SendEvent.m_Owner = shared_from_this(); // ADD_REF

	// ���� �����͸� sendEvent�� ���
	{
		//WRITE_LOCK;

		int32 writeSize = 0;
		while (m_SendQueue.empty() == false)
		{
			CSendBufferRef sendBuffer = m_SendQueue.front();

			writeSize += sendBuffer->WriteSize();
			// TODO : ���� üũ

			m_SendQueue.pop();
			m_SendEvent.m_vecSendBuffer.push_back(sendBuffer);
		}
	}

	// Scatter-Gather (����� �ִ� �����͵��� ��Ƽ� �� �濡 ������)
	std::vector<WSABUF> wsaBufs;
	wsaBufs.reserve(m_SendEvent.m_vecSendBuffer.size());
	for (CSendBufferRef sendBuffer : m_SendEvent.m_vecSendBuffer)
	{
		WSABUF wsaBuf;
		wsaBuf.buf = reinterpret_cast<char*>(sendBuffer->Buffer());
		wsaBuf.len = static_cast<LONG>(sendBuffer->WriteSize());
		wsaBufs.push_back(wsaBuf);
	}

	DWORD numOfBytes = 0;
	if (SOCKET_ERROR == ::WSASend(m_Socket, wsaBufs.data(), static_cast<DWORD>(wsaBufs.size()), OUT &numOfBytes, 0, &m_SendEvent, nullptr))
	{
		int32 errorCode = ::WSAGetLastError();
		if (errorCode != WSA_IO_PENDING)
		{
			HandleError(errorCode);
			m_SendEvent.m_Owner = nullptr; // RELEASE_REF
			m_SendEvent.m_vecSendBuffer.clear(); // RELEASE_REF
			m_bSendRegistered.store(false);
		}
	}
}

void CSession::ProcessConnect()
{
	m_ConnectEvent.m_Owner = nullptr; // RELEASE_REF

	m_bConnected.store(true);

	// ���� ���
	GetService()->AddSession(GetSessionRef());

	// ������ �ڵ忡�� ������
	OnConnected();

	// ���� ���
	RegisterRecv();
}

void CSession::ProcessDisconnect()
{
	m_DisconnectEvent.m_Owner = nullptr; // RELEASE_REF

	OnDisconnected(); // ������ �ڵ忡�� ������
	GetService()->ReleaseSession(GetSessionRef());
}

void CSession::ProcessRecv(int32 numOfBytes)
{
	m_RecvEvent.m_Owner = nullptr; // RELEASE_REF

	if (numOfBytes == 0)
	{
		Disconnect(L"Recv 0");
		return;
	}

	if (m_RecvBuffer.OnWrite(numOfBytes) == false)
	{
		Disconnect(L"OnWrite Overflow");
		return;
	}

	int32 dataSize = m_RecvBuffer.DataSize();
	int32 processLen = OnRecv(m_RecvBuffer.ReadPos(), dataSize); // ������ �ڵ忡�� ������
	if (processLen < 0 || dataSize < processLen || m_RecvBuffer.OnRead(processLen) == false)
	{
		Disconnect(L"OnRead Overflow");
		return;
	}
	
	// Ŀ�� ����
	m_RecvBuffer.Clean();

	// ���� ���
	RegisterRecv();
}

void CSession::ProcessSend(int32 numOfBytes)
{
	m_SendEvent.m_Owner = nullptr; // RELEASE_REF
	m_SendEvent.m_vecSendBuffer.clear(); // RELEASE_REF

	if (numOfBytes == 0)
	{
		Disconnect(L"Send 0");
		return;
	}

	// ������ �ڵ忡�� ������
	OnSend(numOfBytes);

	WRITE_LOCK;

	if (m_SendQueue.empty())
		m_bSendRegistered.store(false);
	else
		RegisterSend();
}

void CSession::HandleError(int32 errorCode)
{
	switch (errorCode)
	{
	case WSAECONNRESET:
	case WSAECONNABORTED:
		Disconnect(L"HandleError");
		break;
	default:
		// TODO : Log
		std::cout << "Handle Error : " << errorCode << std::endl;
		break;
	}
}



CPacketSession::CPacketSession()
{
}

CPacketSession::~CPacketSession()
{
}

// [size(2)][id(2)][data....][size(2)][id(2)][data....]
int32 CPacketSession::OnRecv(BYTE* buffer, int32 len)
{
	int32 processLen = 0;

	while (true)
	{
		int32 dataSize = len - processLen;
		// �ּ��� ����� �Ľ��� �� �־�� �Ѵ�
		if (dataSize < sizeof(PacketHeader))
			break;

		PacketHeader header = *(reinterpret_cast<PacketHeader*>(&buffer[processLen]));
		// ����� ��ϵ� ��Ŷ ũ�⸦ �Ľ��� �� �־�� �Ѵ�
		if (dataSize < header.Size)
			break;

		// ��Ŷ ���� ����
		OnRecvPacket(&buffer[processLen], header.Size);

		processLen += header.Size;
	}

	return processLen;
}
