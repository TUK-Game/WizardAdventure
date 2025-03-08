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

	void DrawLevelWindow();
	void DrawInspectorWindow();
	void DrawGizmo();
	void DrawTagUI();
	void DrawComponentAddUI();
	void DrawTransformUI();
	void DrawRigidbodyUI();
	void DrawMeshRendererUI();

private:
	class CGameObject* m_SelectedObject;  
};

