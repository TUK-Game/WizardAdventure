#pragma once

class CDevice;
class CTimer;


class CEngine :
	public CSingleton<CEngine>
{
	DECLARE_SINGLETON(CEngine)

public:
	ENetwork_Type GetNetworkType() const	{ return m_NetworkType; }
	float GetDeltaTime() const				{ return m_DeltaTime; }
	WindowInfo GetWindowInfo() const		{ return m_WindowInfo; }
	Vec2 GetResolution() const				{ return Vec2((float)m_WindowInfo.Width, (float)m_WindowInfo.Height); }

	void SetNetworkType(ENetwork_Type type)	{ m_NetworkType = type; }

public:
	int Init(HINSTANCE hInstance, HACCEL hAccelTable, const WNDCLASSEXW& wcex, const std::wstring& titleName, int width, int height, bool bWindowed = true);
	int Run();

	void ResizeWindow(int width, int height);

private:
	int CreateMainWindow(const WNDCLASSEXW& wcex);
	void Progress();

private:
	WindowInfo		m_WindowInfo;
	ENetwork_Type	m_NetworkType;	// 서버 연동 키 누르면 바뀌는거라 연결 실패했을 때 재연동 불가능. 다시 접속하려면 클라이언트 다시 켜야함.
	CTimer*			m_Timer;
	float			m_DeltaTime;
};

