#include "pch.h"
#include "Device.h"
#include "RenderManager.h"
#include "Camera.h"
#include "Light.h"
#include "AssetManager.h"
#include "GameObject.h"
#include "ImGuiManager.h"

CRenderManager::CRenderManager()
{
}

CRenderManager::~CRenderManager()
{
}

void CRenderManager::Render()
{
	CImGuiManager::GetInst()->ReadyWindow();

	// Target Clear
	CDevice::GetInst()->RenderBegin();

	PushLightData();

	INT8 backIndex = CDevice::GetInst()->GetSwapChain()->GetBackBufferIndex();
	// swapchain
	CDevice::GetInst()->GetRenderTargetGroup(RENDER_TARGET_GROUP_TYPE::SWAP_CHAIN)->ClearRenderTargetView(backIndex);
	
	// Deferred
	CDevice::GetInst()->GetRenderTargetGroup(RENDER_TARGET_GROUP_TYPE::G_BUFFER)->ClearRenderTargetView();

	// Light
	CDevice::GetInst()->GetRenderTargetGroup(RENDER_TARGET_GROUP_TYPE::LIGHTING)->ClearRenderTargetView();

	CDevice::GetInst()->GetRenderTargetGroup(RENDER_TARGET_GROUP_TYPE::G_BUFFER)->OMSetRenderTargets();

	CCamera* mainCamera = GetMainCamera();
	mainCamera->SortObject();
	mainCamera->Render_Deferred();
	CDevice::GetInst()->GetRenderTargetGroup(RENDER_TARGET_GROUP_TYPE::G_BUFFER)->WaitTargetToResource();

	RenderLights();
	CDevice::GetInst()->GetRenderTargetGroup(RENDER_TARGET_GROUP_TYPE::LIGHTING)->WaitTargetToResource();

	RenderFinal();

	mainCamera->Render_Forward();

	for (auto& camera : m_vecCamera)
	{
		if (camera == mainCamera)
			continue;

		camera->SortObject();
		camera->Render_Forward();
	}

	CImGuiManager::GetInst()->Render();

	// Present
	CDevice::GetInst()->RenderEnd();
}

void CRenderManager::RenderLights()
{
	CDevice::GetInst()->GetRenderTargetGroup(RENDER_TARGET_GROUP_TYPE::LIGHTING)->OMSetRenderTargets();

	for (auto& light : m_vecLight)
	{
		light->Render();
	}
}

void CRenderManager::RenderFinal()
{
	INT8 backIndex = CDevice::GetInst()->GetSwapChain()->GetBackBufferIndex();
	CDevice::GetInst()->GetRenderTargetGroup(RENDER_TARGET_GROUP_TYPE::SWAP_CHAIN)->OMSetRenderTargets(1, backIndex);

	CAssetManager::GetInst()->FindAsset<CMaterial>(L"Final")->GraphicsBinding();
	CAssetManager::GetInst()->FindAsset<CMesh>(L"Rectangle")->Render();
}

void CRenderManager::PushLightData()
{
	LightParams lightParams = {};

	for (auto& light : m_vecLight)
	{
		const LightInfo& lightInfo = light->GetLightInfo();

		light->SetLightIndex(lightParams.lightCount);

		lightParams.lights[lightParams.lightCount] = lightInfo;
		++lightParams.lightCount;
	}

	CONST_BUFFER(EConstantBuffer_Type::Global)->SetGlobalData(&lightParams, sizeof(lightParams));
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

void CRenderManager::RegisterLight(CLight* light)
{
	m_vecLight.push_back(light);
}

void CRenderManager::RemoveObject(CGameObject* obj)
{
	if (obj->GetLight())
	{
		auto findIt = std::find(m_vecLight.begin(), m_vecLight.end(), obj->GetLight());
		if (findIt != m_vecLight.end())
			m_vecLight.erase(findIt);
	}
}

