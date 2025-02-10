#pragma once
class CImGuiManager :
	public CSingleton<CImGuiManager>
{
	DECLARE_SINGLETON(CImGuiManager)
		
public:
	void Init();
	void ReadyWindow();
	void Render();

	void CleanUp();
};

