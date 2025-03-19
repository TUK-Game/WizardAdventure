#pragma once

class CGameObject;

class CImGuiManager :
	public CSingleton<CImGuiManager>
{
	DECLARE_SINGLETON(CImGuiManager)
		
public:
	void Init();
	void ReadyWindow();
	void Render();
	void SetSelectedObject(CGameObject* obj) { m_SelectedObject = obj; }

	void CleanUp();
	void CreateLight(LIGHT_TYPE type);

	void DrawLevelWindow();
	void DrawInspectorWindow();
	void DrawNameUI();
	void DrawGizmo();
	void DrawTagUI();
	void DrawComponentAddUI();
	void DrawTransformUI();
	void DrawRigidbodyUI();
	void DrawMeshRendererUI();
	void DrawLightCreateUI();
	void DrawLightUI();

private:
	class CGameObject* m_SelectedObject;  
};

