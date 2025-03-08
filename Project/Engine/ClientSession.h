#pragma once

#include <ServerCore/Session.h>

class CClientSession :
	public CPacketSession
{
public:
	CClientSession();
	~CClientSession();

public:
	int GetId() const { return m_Id; }

public:
	virtual void OnConnected() override;
	virtual void OnDisconnected() override;
	virtual void OnRecvPacket(BYTE* buffer, int32 len) override;
	virtual void OnSend(int32 len) override;

public:
	int	m_Id;
	std::atomic<std::shared_ptr<class CPlayer>>	m_Player;
};

