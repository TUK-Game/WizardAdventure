#include "pch.h"
#include "Device.h"
#include "Engine.h"
#include "AssetManager.h"

CDevice::CDevice()
	: m_Viewport{}
	, m_ScissorRect{}
{
}

CDevice::~CDevice()
{
}

int CDevice::Init()
{
	m_Viewport = { 0, 0, static_cast<FLOAT>(CEngine::GetInst()->GetWindowInfo().Width), static_cast<FLOAT>(CEngine::GetInst()->GetWindowInfo().Height), 0.0f, 1.0f};
	m_ScissorRect = CD3DX12_RECT(0, 0, CEngine::GetInst()->GetWindowInfo().Width, CEngine::GetInst()->GetWindowInfo().Height);

#ifdef _DEBUG
	::D3D12GetDebugInterface(IID_PPV_ARGS(&m_DebugController));
	m_DebugController->EnableDebugLayer();
#endif

	::CreateDXGIFactory(IID_PPV_ARGS(&m_DXGI));
	::D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&m_Device));

	m_GraphicsCmdQueue = std::make_shared<CGraphicsCommandQueue>();
	m_ComputeCmdQueue = std::make_shared<CComputeCommandQueue>();
	m_SwapChain = std::make_shared<CSwapChain>();
	m_RootSignature = std::make_shared<CRootSignature>();
	m_GraphicsDescHeap = std::make_shared<CGraphicsDescriptorHeap>();
	m_ComputeDescHeap = std::make_shared<CComputeDescriptorHeap>();

	if (FAILED(m_GraphicsCmdQueue->Init(m_Device, m_SwapChain)))
		return E_FAIL;

	if (FAILED(m_ComputeCmdQueue->Init(m_Device)))
		return E_FAIL;

	if (FAILED(m_SwapChain->Init(CEngine::GetInst()->GetWindowInfo(), m_Device, m_DXGI, m_GraphicsCmdQueue->GetCmdQueue())))
		return E_FAIL;

	if (FAILED(m_RootSignature->Init()))
		return E_FAIL;

	if (FAILED(m_GraphicsDescHeap->Init(1000)))
		return E_FAIL;

	if (FAILED(m_ComputeDescHeap->Init()))
		return E_FAIL;

	// 사용할 수에 맞게 레지스터 개수 생성 ex) 렌더링 오브젝트 2개면 각 2개씩
	CreateConstantBuffer(CBV_REGISTER::b0, sizeof(TransformParams), 1000);
	CreateConstantBuffer(CBV_REGISTER::b1, sizeof(MaterialParams), 1000);

	CreateRenderTargetGroups();
	return S_OK;
}

void CDevice::RenderBegin()	
{
	m_GraphicsCmdQueue->RenderBegin(&m_Viewport, &m_ScissorRect);
}

void CDevice::RenderEnd()
{
	m_GraphicsCmdQueue->RenderEnd();
}

void CDevice::CreateConstantBuffer(CBV_REGISTER reg, UINT bufferSize, UINT count)
{
	unsigned char typeInt = static_cast<unsigned char>(reg);
	assert(m_ConstantBuffers.size() == typeInt);

	std::shared_ptr<CConstantBuffer> buffer = std::make_shared<CConstantBuffer>();
	buffer->Init(reg, bufferSize, count);
	m_ConstantBuffers.push_back(buffer);
}

void CDevice::CreateRenderTargetGroups()
{
	// DepthStencil
	CTexture* dsTexture = CAssetManager::GetInst()->CreateTexture(L"DepthStencil",
		DXGI_FORMAT_D32_FLOAT, CEngine::GetInst()->GetWindowInfo(),
		CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE, D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL);

	// SwapChain Group
	{
		std::vector<RenderTarget> rtVec(SWAP_CHAIN_BUFFER_COUNT);

		for (UINT32 i = 0; i < SWAP_CHAIN_BUFFER_COUNT; ++i)
		{
			std::wstring name = L"SwapChainTarget_" + std::to_wstring(i);

			ComPtr<ID3D12Resource> resource;
			m_SwapChain->GetSwapChain()->GetBuffer(i, IID_PPV_ARGS(&resource));
			rtVec[i].target = CAssetManager::GetInst()->CreateTextureFromResource(name, resource);
		}

		m_RenderTargetGroups[static_cast<UINT8>(RENDER_TARGET_GROUP_TYPE::SWAP_CHAIN)] = std::make_shared<CRenderTargetGroup>();
		m_RenderTargetGroups[static_cast<UINT8>(RENDER_TARGET_GROUP_TYPE::SWAP_CHAIN)]->Create(RENDER_TARGET_GROUP_TYPE::SWAP_CHAIN, rtVec, dsTexture);
	}

	// Deferred Group
	{
		std::vector<RenderTarget> rtVec(RENDER_TARGET_G_BUFFER_GROUP_MEMBER_COUNT);

		rtVec[0].target = CAssetManager::GetInst()->CreateTexture(L"PositionTarget",
			DXGI_FORMAT_R32G32B32A32_FLOAT, CEngine::GetInst()->GetWindowInfo(),
			CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
			D3D12_HEAP_FLAG_NONE, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET);

		rtVec[1].target = CAssetManager::GetInst()->CreateTexture(L"NormalTarget",
			DXGI_FORMAT_R32G32B32A32_FLOAT, CEngine::GetInst()->GetWindowInfo(),
			CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
			D3D12_HEAP_FLAG_NONE, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET);

		rtVec[2].target = CAssetManager::GetInst()->CreateTexture(L"DiffuseTarget",
			DXGI_FORMAT_R8G8B8A8_UNORM, CEngine::GetInst()->GetWindowInfo(),
			CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
			D3D12_HEAP_FLAG_NONE, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET);

		m_RenderTargetGroups[static_cast<UINT8>(RENDER_TARGET_GROUP_TYPE::G_BUFFER)] = std::make_shared<CRenderTargetGroup>();
		m_RenderTargetGroups[static_cast<UINT8>(RENDER_TARGET_GROUP_TYPE::G_BUFFER)]->Create(RENDER_TARGET_GROUP_TYPE::G_BUFFER, rtVec, dsTexture);
	}
}
