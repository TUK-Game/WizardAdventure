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
	CGameObject* GetPlayer() { return m_Player; }
	void SetPlayer(CGameObject* player) { m_Player = player; }

private:
	void CleanUpCurrentLevel();

public:
	class CGameObject* Pick(INT32 x, INT32 y);

private:
	CLevel* m_CurLevel = nullptr;
	CLevel* m_NextLevel = nullptr;
	float m_timer = 0.f;
	class CGameObject* m_Player;
};

