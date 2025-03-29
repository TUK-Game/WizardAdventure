#pragma once
#include "IocpCore.h"
#include "NetAddress.h"

class CAcceptEvent;
class CServerService;

class CListener : public CIocpObject
{
public:
	CListener() = default;
	~CListener();

public:
	/* �ܺο��� ��� */
	bool StartAccept(CServerServiceRef service);
	void CloseSocket();

public:
	/* �������̽� ���� */
	virtual HANDLE GetHandle() override;
	virtual void Dispatch(class CIocpEvent* iocpEvent, int32 numOfBytes = 0) override;

private:
	/* ���� ���� */
	void RegisterAccept(CAcceptEvent* acceptEvent);
	void ProcessAccept(CAcceptEvent* acceptEvent);

protected:
	SOCKET m_Socket = INVALID_SOCKET;
	std::vector<CAcceptEvent*> m_vecAcceptEvent;
	CServerServiceRef m_Service;
};

