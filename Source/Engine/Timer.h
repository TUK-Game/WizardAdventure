#pragma once

class CTimer
{
public:
	CTimer();
	~CTimer();

public:
	float GetDeltaTime() const	{ return m_DeltaTime; }

public:
	void Init();
	void Update();

private:
	std::chrono::steady_clock::time_point	m_CurTime;
	std::chrono::steady_clock::time_point	m_PrevTime;
	UINT			m_FPS;
	float			m_DeltaTime;
	float			m_Time;
};

