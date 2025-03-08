#pragma once
#include "IocpCore.h"
#include "IocpEvent.h"
#include "NetAddress.h"
#include "RecvBuffer.h"

class CService;

class CSession : public CIocpObject
{
	friend class CListener;
	friend class CIocpCore;
	friend class CService;

	enum
	{
		BUFFER_SIZE = 0x10000, // 64KB
	};

public:
	CSession();
	virtual ~CSession();

public:
						/* 외부에서 사용 */
	void				Send(CSendBufferRef sendBuffer);
	bool				Connect();
	void				Disconnect(const WCHAR* cause);

	std::shared_ptr<CService>	GetService() { return m_Service.lock(); }
	void				SetService(std::shared_ptr<CService> service) { m_Service = service; }

public:
						/* 정보 관련 */
	void				SetNetAddress(CNetAddress address) { m_NetAddress = address; }
	CNetAddress			GetAddress() { return m_NetAddress; }
	SOCKET				GetSocket() { return m_Socket; }
	bool				IsConnected() { return m_bConnected; }
	CSessionRef			GetSessionRef() { return static_pointer_cast<CSession>(shared_from_this()); }

private:
						/* 인터페이스 구현 */
	virtual HANDLE		GetHandle() override;
	virtual void		Dispatch(class CIocpEvent* iocpEvent, int32 numOfBytes = 0) override;

private:
						/* 전송 관련 */
	bool				RegisterConnect();
	bool				RegisterDisconnect();
	void				RegisterRecv();
	void				RegisterSend();

	void				ProcessConnect();
	void				ProcessDisconnect();
	void				ProcessRecv(int32 numOfBytes);
	void				ProcessSend(int32 numOfBytes);

	void				HandleError(int32 errorCode);

protected:
						/* 컨텐츠 코드에서 재정의 */
	virtual void		OnConnected() { }
	virtual int32		OnRecv(BYTE* buffer, int32 len) { return len; }
	virtual void		OnSend(int32 len) { }
	virtual void		OnDisconnected() { }

private:
	std::weak_ptr<CService>		m_Service;
	SOCKET						m_Socket = INVALID_SOCKET;
	CNetAddress					m_NetAddress = {};
	std::atomic<bool>			m_bConnected = false;

private:
	USE_LOCK;
							/* 수신 관련 */
	CRecvBuffer					m_RecvBuffer;

							/* 송신 관련 */
	std::queue<CSendBufferRef>	m_SendQueue;
	std::atomic<bool>			m_bSendRegistered = false;

private:
						/* IocpEvent 재사용 */
	CConnectEvent		m_ConnectEvent;
	CDisconnectEvent	m_DisconnectEvent;
	CRecvEvent			m_RecvEvent;
	CSendEvent			m_SendEvent;
};

/*-----------------
	PacketSession
------------------*/

struct PacketHeader
{
	uint16 Size;
	uint16 Id; // 프로토콜ID (ex. 1=로그인, 2=이동요청)
};

class CPacketSession : public CSession
{
public:
	CPacketSession();
	virtual ~CPacketSession();

	CPacketSessionRef	GetPacketSessionRef() { return static_pointer_cast<CPacketSession>(shared_from_this()); }

protected:
	virtual int32		OnRecv(BYTE* buffer, int32 len) final;
	virtual void		OnRecvPacket(BYTE* buffer, int32 len) = 0;
};