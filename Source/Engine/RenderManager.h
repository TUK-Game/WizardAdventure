#pragma once

class CCamera;
class CLight;


class CRenderManager :
	public CSingleton<CRenderManager>
{
	DECLARE_SINGLETON(CRenderManager)

public:
	CCamera* GetMainCamera() { return m_vecCamera[0]; }
public:
	void ClearRTV();
	void Render();
	void RenderShadow();
	void RenderDeferred();
	void RenderLights();
	void RenderFinal();
	void RenderForward();
public:
	void RegisterCamera(CCamera* camera, int priority);
	void RegisterLight(CLight* light);

	void PushLightData();
	void RemoveObject(class CGameObject* obj);
private:
	std::vector<CCamera*>	m_vecCamera;
	std::vector<CLight*>	m_vecLight;
};

