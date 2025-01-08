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
