#pragma once

#include <ServerCore/Session.h>

class CServerSession :
	public CPacketSession
{
public:
	CServerSession();
	~CServerSession();

public:
	void SetOwnPlayer(class CPlayer* p) { m_OwnPlayer = p; }
	void SetClientID(UINT64 id) { m_Id = id; }

	virtual void OnConnected() override;
	virtual void OnDisconnected() override;
	virtual void OnRecvPacket(BYTE* buffer, int32 len) override;
	virtual void OnSend(int32 len) override;

	void OnMovePlayer();
	void OnActPlayer();

private:
	class CPlayer* m_OwnPlayer;
	UINT64			m_Id;
};