#pragma once

class CEditorManager :
	public CSingleton<CEditorManager>
{
	DECLARE_SINGLETON(CEditorManager)

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

