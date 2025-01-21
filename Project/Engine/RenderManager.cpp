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

	INT8 backIndex = CDevice::GetInst()->GetSwapChain()->GetBackBufferIndex();
	// swapchain
	CDevice::GetInst()->GetRenderTargetGroup(RENDER_TARGET_GROUP_TYPE::SWAP_CHAIN)->ClearRenderTargetView(backIndex);
	
	// Deferred
	CDevice::GetInst()->GetRenderTargetGroup(RENDER_TARGET_GROUP_TYPE::G_BUFFER)->ClearRenderTargetView();

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
