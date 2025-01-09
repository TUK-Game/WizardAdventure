#pragma once

class CCamera;


class CRenderManager :
	public CSingleton<CRenderManager>
{
	DECLARE_SINGLETON(CRenderManager)

public:
	void Render();

public:
	void RegisterCamera(CCamera* camera, int priority);

private:
	std::vector<CCamera*>	m_vecCamera;
};

