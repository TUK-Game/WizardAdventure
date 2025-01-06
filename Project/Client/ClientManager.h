#pragma once

class CClientManager :
	public CSingleton<CClientManager>
{
	DECLARE_SINGLETON(CClientManager)

public:
	int Init(HINSTANCE instance);
	int Run();

private:
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

private:
	int InitEngine();

private:
	HINSTANCE	m_hInstance;
};

