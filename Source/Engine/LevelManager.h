#pragma once

class CLevel;


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

private:
	void CleanUpCurrentLevel();

public:
	class CGameObject* Pick(INT32 x, INT32 y);

private:
	class CLevel* m_CurLevel;
	float m_timer = 0.f;
};

