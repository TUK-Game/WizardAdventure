#pragma once

#include "Session.h"

class CPlayer;

class CGameSession : public CPacketSession
{
public:
	CGameSession();
	~CGameSession();

public:
	int GetId() const { return Id; }

public:
	virtual void OnConnected() override;
	virtual void OnDisconnected() override;
	virtual void OnRecvPacket(BYTE* buffer, int32 len) override;
	virtual void OnSend(int32 len) override;

public:
	int	Id;
	std::atomic<std::shared_ptr<CPlayer>>	Player;
};