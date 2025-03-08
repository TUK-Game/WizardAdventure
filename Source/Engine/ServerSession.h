#pragma once

#include <ServerCore/Session.h>

class CServerSession :
	public CPacketSession
{
public:
	CServerSession();
	~CServerSession();

	virtual void OnConnected() override;
	virtual void OnDisconnected() override;
	virtual void OnRecvPacket(BYTE* buffer, int32 len) override;
	virtual void OnSend(int32 len) override;
};