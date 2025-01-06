#pragma once

class CEngine :
	public CSingleton<CEngine>
{
	DECLARE_SINGLETON(CEngine)

public:
	int Init(HINSTANCE hInstance, HACCEL hAccelTable, const WNDCLASSEXW& wcex, const std::wstring& titleName, int width, int height);
	int Run();

private:
	int CreateMainWindow(const WNDCLASSEXW& wcex);
	void Progress();

private:
	HINSTANCE		m_hInstance;
	HACCEL			m_hAccelTable;
	HWND			m_hMainWnd;
	HDC				m_hMainDC;
	std::wstring	m_TitleName;
	POINT			m_Resolution;
};

