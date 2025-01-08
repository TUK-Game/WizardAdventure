#pragma once

class CCamera;


class CRenderManager :
	public CSingleton<CRenderManager>
{
	DECLARE_SINGLETON(CRenderManager)

public:
	void Render();

public:
	// TODO: 카메라 등록

private:
	std::vector<CCamera*>	m_vecCamera;
};

