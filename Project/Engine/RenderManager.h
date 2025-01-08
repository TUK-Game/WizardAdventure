#pragma once

class CCamera;


class CRenderManager :
	public CSingleton<CRenderManager>
{
	DECLARE_SINGLETON(CRenderManager)

public:
	void Render();

public:
	// TODO: ī�޶� ���

private:
	std::vector<CCamera*>	m_vecCamera;
};

