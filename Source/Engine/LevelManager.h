#pragma once

class CLevel;


class CLevelManager
	: public CSingleton<CLevelManager>
{
	DECLARE_SINGLETON(CLevelManager)

public:
	CLevel* GetCurrentLevel()	{ return m_CurLevel; }

public:
	int Init();
	void Progress();

public:
	class CGameObject* Pick(INT32 x, INT32 y);

private:
	class CLevel* m_CurLevel;
};

