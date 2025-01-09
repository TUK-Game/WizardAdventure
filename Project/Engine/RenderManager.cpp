#include "pch.h"
#include "Device.h"
#include "RenderManager.h"
#include "Camera.h"

CRenderManager::CRenderManager()
{
}

CRenderManager::~CRenderManager()
{
}

void CRenderManager::Render()
{
	// Target Clear
	CDevice::GetInst()->RenderBegin();

	for (auto& camera : m_vecCamera)
	{
		camera->Render();
	}

	// Present
	CDevice::GetInst()->RenderEnd();
}

void CRenderManager::RegisterCamera(CCamera* camera, int priority)
{
	if (m_vecCamera.size() <= priority)
	{
		m_vecCamera.resize(priority + 1);
	}

	if (m_vecCamera[priority] != nullptr)
	{
		m_vecCamera[priority]->m_Priority = -1;
	}

	m_vecCamera[priority] = camera;
}
