#include "pch.h"
#include "Device.h"
#include "RenderManager.h"
#include "Camera.h"
#include "Light.h"
#include "AssetManager.h"
#include "GameObject.h"
#include "ImGuiManager.h"
#include "Transform.h"

CRenderManager::CRenderManager()
{
}

CRenderManager::~CRenderManager()
{
}

void CRenderManager::Render()
{
#ifdef READY_IMGUI
	CImGuiManager::GetInst()->ReadyWindow();
#endif
	ClearRTV();

	PushLightData();

	PushFogData();

	RenderShadow();

	RenderDeferred();
	
	RenderLights();

	//RenderMap();

	RenderFinal();

	RenderForward();

	//for (auto& camera : m_vecCamera)
	//{
	//	camera->Render();
	//}
#ifdef READY_IMGUI
	CImGuiManager::GetInst()->Render();
#endif
	// Present
	CDevice::GetInst()->RenderEnd();
}

void CRenderManager::ClearRTV()
{
	// Target Clear
	CDevice::GetInst()->RenderBegin();

	INT8 backIndex = CDevice::GetInst()->GetSwapChain()->GetBackBufferIndex();
	// swapchain
	CDevice::GetInst()->GetRenderTargetGroup(RENDER_TARGET_GROUP_TYPE::SWAP_CHAIN)->ClearRenderTargetView(backIndex);

	// Shadow 
	CDevice::GetInst()->GetRenderTargetGroup(RENDER_TARGET_GROUP_TYPE::SHADOW)->ClearRenderTargetView();

	// Deferred
	CDevice::GetInst()->GetRenderTargetGroup(RENDER_TARGET_GROUP_TYPE::G_BUFFER)->ClearRenderTargetView();

	// Light
	CDevice::GetInst()->GetRenderTargetGroup(RENDER_TARGET_GROUP_TYPE::LIGHTING)->ClearRenderTargetView();

	// Map
	//CDevice::GetInst()->GetRenderTargetGroup(RENDER_TARGET_GROUP_TYPE::MAP)->ClearRenderTargetView();
}

void CRenderManager::RenderShadow()
{
	CDevice::GetInst()->GetRenderTargetGroup(RENDER_TARGET_GROUP_TYPE::SHADOW)->OMSetRenderTargets();

	for (auto& light : m_vecLight)
	{
		if (light->GetLightType() != LIGHT_TYPE::DIRECTIONAL_LIGHT)
			continue;

		light->RenderShadow();
	}

	CDevice::GetInst()->GetRenderTargetGroup(RENDER_TARGET_GROUP_TYPE::SHADOW)->WaitTargetToResource();
}

void CRenderManager::RenderMap(CGameObject* object)
{
	CDevice::GetInst()->GetRenderTargetGroup(RENDER_TARGET_GROUP_TYPE::MAP)->OMSetRenderTargets();

	CCamera* mapCamera = m_vecCamera[2];
	mapCamera->SortObject();
	mapCamera->RenderMap(object);

	CDevice::GetInst()->GetRenderTargetGroup(RENDER_TARGET_GROUP_TYPE::MAP)->WaitTargetToResource();
}

void CRenderManager::RenderDeferred()
{
	CDevice::GetInst()->GetRenderTargetGroup(RENDER_TARGET_GROUP_TYPE::G_BUFFER)->OMSetRenderTargets();

	CCamera* mainCamera = GetMainCamera();
	mainCamera->SortObject();
	mainCamera->RenderDeferred();
	CDevice::GetInst()->GetRenderTargetGroup(RENDER_TARGET_GROUP_TYPE::G_BUFFER)->WaitTargetToResource();
}

void CRenderManager::RenderLights()
{
	CCamera* mainCamera = GetMainCamera();
	CCamera::s_matView = mainCamera->GetViewMat();
	CCamera::s_matProjection = mainCamera->GetProjMat();

	CDevice::GetInst()->GetRenderTargetGroup(RENDER_TARGET_GROUP_TYPE::LIGHTING)->OMSetRenderTargets();

	for (auto& light : m_vecLight)
	{
		light->Render();
	}

	CDevice::GetInst()->GetRenderTargetGroup(RENDER_TARGET_GROUP_TYPE::LIGHTING)->WaitTargetToResource();
}

void CRenderManager::RenderFinal()
{
	INT8 backIndex = CDevice::GetInst()->GetSwapChain()->GetBackBufferIndex();
	CDevice::GetInst()->GetRenderTargetGroup(RENDER_TARGET_GROUP_TYPE::SWAP_CHAIN)->OMSetRenderTargets(1, backIndex);

	CAssetManager::GetInst()->FindAsset<CMaterial>(L"Final")->GraphicsBinding();
	CAssetManager::GetInst()->FindAsset<CMesh>(L"Rectangle")->Render();
}

void CRenderManager::RenderForward()
{
	CCamera* mainCamera = GetMainCamera();
	mainCamera->RenderForward();

	for (auto& camera : m_vecCamera)
	{
		if (camera == mainCamera || m_vecCamera[2] == camera)
			continue;

		camera->SortObject();
		camera->RenderForward();
	}
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

	CONST_BUFFER(EConstantBuffer_Type::Global)->SetGlobalData(&lightParams, sizeof(lightParams), 0);
}

void CRenderManager::PushFogData()
{
	FogParams params{};
	params.cameraPosition = GetMainCamera()->GetTransform()->GetWorldPosition();
	params.fogColor = Vec4(0.5f, 0.5f, 0.5f, 1);
	params.Start = 2000;
	params.range = 4000 - 2000; // End - Start

	CONST_BUFFER(EConstantBuffer_Type::Fog)->SetGlobalData(&params, sizeof(params), 1);
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

