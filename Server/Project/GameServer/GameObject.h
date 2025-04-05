#pragma once
class CBoxCollider;

class CGameObject : public std::enable_shared_from_this<CGameObject>
{
	friend class CRoom;
	friend class CObjectUtil;

public:
	CGameObject();
	virtual ~CGameObject();

public:
	std::shared_ptr<CRoom> GetRoom() const	{ return m_Room.load().lock(); }
	CBoxCollider* GetCollider() const { return m_BoxCollider; }

	bool IsPlayer() const	{ return m_bPlayer; }


public:
	virtual void Update();

	virtual void CollisionBegin(CBoxCollider* src, CBoxCollider* dest) {}
	virtual void CollisionEnd(CBoxCollider* src, CBoxCollider* dest) {}

public:	// public 이라서 m_ 안붙임. (ex. struct 필드)
	Protocol::ObjectInfo*	ObjectInfo;
	Protocol::PosInfo*		PosInfo;	

protected:
	std::atomic<std::weak_ptr<CRoom>>	m_Room;

protected:
	bool	m_bPlayer;
	CBoxCollider* m_BoxCollider;
};

