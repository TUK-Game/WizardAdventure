#pragma once

class CLevel;
class CGameObject;


class CLevelManager
	: public CSingleton<CLevelManager>
{
	DECLARE_SINGLETON(CLevelManager)

public:
	CLevel* GetCurrentLevel()	{ return m_CurLevel; }
	void SetLevel(CLevel* level) { m_CurLevel = level; }

public:
	int Init();
	void Progress();
	void ChangeLevel(CLevel* newLevel);
	CGameObject* GetOwnPlayer() { return m_OwnPlayer; }
	CGameObject* GetPlayer(UINT64 playerId) { return m_Players[playerId]; }
	void SetOwnPlayer(CGameObject* player) { m_OwnPlayer = player; }
	void SetPlayer(CGameObject* player, UINT64 playerId) { m_Players[playerId] = player; }

private:	
	void CleanUpCurrentLevel();

public:
	class CGameObject* Pick(INT32 x, INT32 y);

private:
	class CLevel* m_CurLevel;
	float m_timer = 0.f;
	class CGameObject* m_OwnPlayer;
	std::array<CGameObject*, MAX_PLAYERS> m_Players;
};

