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
	void SetClientID(UINT64 id) { m_Id = id; m_projectileId = m_Id * 1000000 + 1; }

	class CPlayer* GetOwnPlayer() { return m_OwnPlayer; }
	UINT64 GetClientID() { return m_Id; }

	virtual void OnConnected() override;
	virtual void OnDisconnected() override;
	virtual void OnRecvPacket(BYTE* buffer, int32 len) override;
	virtual void OnSend(int32 len) override;

public:
	void SelectMageAttribute(EPlayerAttribute attribute);

	void OnMovePlayer();
	void OnActPlayer();
	void MoveSkill(class CSkillObject* object);
	void SpawnSkill(class CSkillObject* object);

	void BuyItem(uint32 itemId);
private:
	class CPlayer* m_OwnPlayer;
	UINT64			m_Id;
	UINT64			m_projectileId	;
};

