#pragma once

class CDevice;

class CEngine :
	public CSingleton<CEngine>
{
	DECLARE_SINGLETON(CEngine)

public:
	WindowInfo GetWindowInfo() const	{ return m_WindowInfo; }

public:
	int Init(HINSTANCE hInstance, HACCEL hAccelTable, const WNDCLASSEXW& wcex, const std::wstring& titleName, int width, int height, bool bWindowed = true);
	int Run();

	void ResizeWindow(int width, int height);

private:
	int CreateMainWindow(const WNDCLASSEXW& wcex);
	void Progress();

private:
	WindowInfo					m_WindowInfo;
};

