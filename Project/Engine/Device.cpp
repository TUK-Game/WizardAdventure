#include "pch.h"
#include "Device.h"
#include "Engine.h"
#include "AssetManager.h"
#include "ImGuiManager.h"

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

	::CreateDXGIFactory(IID_PPV_ARGS(&m_DXGI));
	::D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&m_Device));

	m_GraphicsCmdQueue = std::make_shared<CGraphicsCommandQueue>();
	m_ComputeCmdQueue = std::make_shared<CComputeCommandQueue>();
	m_SwapChain = std::make_shared<CSwapChain>();
	m_RootSignature = std::make_shared<CRootSignature>();
	m_GraphicsDescHeap = std::make_shared<CGraphicsDescriptorHeap>();
	m_ImGuiDescHeap = std::make_shared<CGraphicsDescriptorHeap>();
	m_ComputeDescHeap = std::make_shared<CComputeDescriptorHeap>();

	if (FAILED(m_GraphicsCmdQueue->Init(m_Device, m_SwapChain)))
		return E_FAIL;

	if (FAILED(m_ComputeCmdQueue->Init(m_Device)))
		return E_FAIL;

	if (FAILED(m_SwapChain->Init(CEngine::GetInst()->GetWindowInfo(), m_Device, m_DXGI, m_GraphicsCmdQueue->GetCmdQueue())))
		return E_FAIL;

	if (FAILED(m_RootSignature->Init()))
		return E_FAIL;

	if (FAILED(m_GraphicsDescHeap->Init(40000)))
		return E_FAIL;

	if (FAILED(m_ImGuiDescHeap->Init(1000)))
		return E_FAIL;

	if (FAILED(m_ComputeDescHeap->Init()))
		return E_FAIL;

	// 사용할 수에 맞게 레지스터 개수 생성 ex) 렌더링 오브젝트 2개면 각 2개씩
	CreateConstantBuffer(CBV_REGISTER::b0, sizeof(LightParams), 1);
	CreateConstantBuffer(CBV_REGISTER::b1, sizeof(FogParams), 1);
	CreateConstantBuffer(CBV_REGISTER::b2, sizeof(TransformParams), 400000);
	CreateConstantBuffer(CBV_REGISTER::b3, sizeof(MaterialParams), 400000);

	CreateRenderTargetGroups();
#ifdef READY_IMGUI
	CImGuiManager::GetInst()->Init();
#endif


	//ComPtr<ID3D12InfoQueue> infoQueue;
	//m_Device->QueryInterface(IID_PPV_ARGS(&infoQueue));
	//infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, TRUE);

	Create2DDevice();

	return S_OK;
}

void CDevice::RenderBegin()	
{
	m_GraphicsCmdQueue->RenderBegin();
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
		DXGI_FORMAT_D32_FLOAT, CEngine::GetInst()->GetWindowInfo().Width, CEngine::GetInst()->GetWindowInfo().Height,
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

	// Shadow Group
	{
		std::vector<RenderTarget> rtVec(RENDER_TARGET_SHADOW_GROUP_MEMBER_COUNT);

		rtVec[0].target = CAssetManager::GetInst()->CreateTexture(L"ShadowTarget",
			DXGI_FORMAT_R32_FLOAT, 4096, 4096,
			CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
			D3D12_HEAP_FLAG_NONE, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET);

		CTexture* shadowDepthTexture = CAssetManager::GetInst()->CreateTexture(L"ShadowDepthStencil",
			DXGI_FORMAT_D32_FLOAT, 4096, 4096,
			CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
			D3D12_HEAP_FLAG_NONE, D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL);

		m_RenderTargetGroups[static_cast<UINT8>(RENDER_TARGET_GROUP_TYPE::SHADOW)] = std::make_shared<CRenderTargetGroup>();
		m_RenderTargetGroups[static_cast<UINT8>(RENDER_TARGET_GROUP_TYPE::SHADOW)]->Create(RENDER_TARGET_GROUP_TYPE::SHADOW, rtVec, shadowDepthTexture);
	}

	// Deferred Group
	{
		std::vector<RenderTarget> rtVec(RENDER_TARGET_G_BUFFER_GROUP_MEMBER_COUNT);

		rtVec[0].target = CAssetManager::GetInst()->CreateTexture(L"PositionTarget",
			DXGI_FORMAT_R32G32B32A32_FLOAT, CEngine::GetInst()->GetWindowInfo().Width, CEngine::GetInst()->GetWindowInfo().Height,
			CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
			D3D12_HEAP_FLAG_NONE, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET);

		rtVec[1].target = CAssetManager::GetInst()->CreateTexture(L"NormalTarget",
			DXGI_FORMAT_R32G32B32A32_FLOAT, CEngine::GetInst()->GetWindowInfo().Width, CEngine::GetInst()->GetWindowInfo().Height,
			CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
			D3D12_HEAP_FLAG_NONE, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET);

		rtVec[2].target = CAssetManager::GetInst()->CreateTexture(L"DiffuseTarget",
			DXGI_FORMAT_R8G8B8A8_UNORM, CEngine::GetInst()->GetWindowInfo().Width, CEngine::GetInst()->GetWindowInfo().Height,
			CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
			D3D12_HEAP_FLAG_NONE, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET);

		m_RenderTargetGroups[static_cast<UINT8>(RENDER_TARGET_GROUP_TYPE::G_BUFFER)] = std::make_shared<CRenderTargetGroup>();
		m_RenderTargetGroups[static_cast<UINT8>(RENDER_TARGET_GROUP_TYPE::G_BUFFER)]->Create(RENDER_TARGET_GROUP_TYPE::G_BUFFER, rtVec, dsTexture);
	}

	// Lighting Group
	{
		std::vector<RenderTarget> rtVec(RENDER_TARGET_LIGHTING_GROUP_MEMBER_COUNT);

		rtVec[0].target = CAssetManager::GetInst()->CreateTexture(L"DiffuseLightTarget",
			DXGI_FORMAT_R8G8B8A8_UNORM, CEngine::GetInst()->GetWindowInfo().Width, CEngine::GetInst()->GetWindowInfo().Height,
			CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
			D3D12_HEAP_FLAG_NONE, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET);

		rtVec[1].target = CAssetManager::GetInst()->CreateTexture(L"SpecularLightTarget",
			DXGI_FORMAT_R8G8B8A8_UNORM, CEngine::GetInst()->GetWindowInfo().Width, CEngine::GetInst()->GetWindowInfo().Height,
			CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
			D3D12_HEAP_FLAG_NONE, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET);

		m_RenderTargetGroups[static_cast<UINT8>(RENDER_TARGET_GROUP_TYPE::LIGHTING)] = std::make_shared<CRenderTargetGroup>();
		m_RenderTargetGroups[static_cast<UINT8>(RENDER_TARGET_GROUP_TYPE::LIGHTING)]->Create(RENDER_TARGET_GROUP_TYPE::LIGHTING, rtVec, dsTexture);
	}

	// Map Group
	{
		std::vector<RenderTarget> rtVec(RENDER_TARGET_MAP_GROUP_MEMBER_COUNT);

		rtVec[0].target = CAssetManager::GetInst()->CreateTexture(L"MiniMapTarget",
			DXGI_FORMAT_R32G32B32A32_FLOAT, CEngine::GetInst()->GetWindowInfo().Width, CEngine::GetInst()->GetWindowInfo().Height,
			CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
			D3D12_HEAP_FLAG_NONE, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET);

		m_RenderTargetGroups[static_cast<UINT8>(RENDER_TARGET_GROUP_TYPE::MAP)] = std::make_shared<CRenderTargetGroup>();
		m_RenderTargetGroups[static_cast<UINT8>(RENDER_TARGET_GROUP_TYPE::MAP)]->Create(RENDER_TARGET_GROUP_TYPE::MAP, rtVec, dsTexture);
	}
}

void CDevice::Create2DDevice()
{
#pragma region Text
	ID3D11Device* pd3d11Device = NULL;
	ID3D12CommandQueue* ppd3dCommandQueues[] = { GetCmdQueue()->GetCmdQueue().Get() };
	HRESULT hResult = ::D3D11On12CreateDevice(m_Device.Get(),
		D3D11_CREATE_DEVICE_BGRA_SUPPORT,
		NULL,
		0,
		reinterpret_cast<IUnknown**>(ppd3dCommandQueues),
		1,
		0,
		&pd3d11Device,
		&m_DeviceContext,
		NULL);
	hResult = pd3d11Device->QueryInterface(IID_PPV_ARGS(&m_d3d11On12Device));
	if (pd3d11Device) pd3d11Device->Release();

	D2D1_FACTORY_OPTIONS factoryOption = { D2D1_DEBUG_LEVEL_NONE };
	hResult = ::D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED,
		__uuidof(ID2D1Factory3), &factoryOption, (void**)&m_d2dFactory);

	IDXGIDevice* pdxgiDevice = NULL;
	hResult = m_d3d11On12Device->QueryInterface(IID_PPV_ARGS(&pdxgiDevice));
	if (pdxgiDevice != NULL)
		hResult = m_d2dFactory->CreateDevice(pdxgiDevice, &m_d2dDevice);

	hResult = m_d2dDevice->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_NONE, &m_d2dDeviceContext);
	hResult = ::DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), (IUnknown**)&m_WriteFactory);
	if (pdxgiDevice) pdxgiDevice->Release();


	m_d2dDeviceContext->SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE_GRAYSCALE);

	m_d2dDeviceContext->CreateSolidColorBrush(D2D1::ColorF(0.3f, 0.0f, 0.0f, 0.5f), &m_d2dbrBackground);
	m_d2dDeviceContext->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF(0x9ACD32, 1.0f)), &m_d2dbrBorder);
	AddFont();
	AddColor();
	hResult = m_WriteFactory->CreateTextLayout(L"텍스트 레이아웃", 8, m_dwFont.Get(), 4096.0f, 4096.0f, &m_dwTextLayout);

	float fDpi = (float)GetDpiForWindow(CEngine::GetInst()->GetWindowInfo().hWnd);
	D2D1_BITMAP_PROPERTIES1 d2dBitmapProperties = D2D1::BitmapProperties1(D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW, D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED), fDpi, fDpi);

	auto swapchain = GetSwapChain()->GetSwapChain();
	for (UINT i = 0; i < SWAP_CHAIN_BUFFER_COUNT; i++)
	{
		ComPtr<ID3D12Resource> r;
		hResult = swapchain->GetBuffer(i, IID_PPV_ARGS(&r));
		D3D11_RESOURCE_FLAGS d3d11Flags = { D3D11_BIND_RENDER_TARGET };
		m_d3d11On12Device->CreateWrappedResource(r.Get(), &d3d11Flags,
			D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT, IID_PPV_ARGS(&m_d3d11WrappedBackBuffers[i]));

		IDXGISurface* pdxgiSurface = NULL;

		m_d3d11WrappedBackBuffers[i]->QueryInterface(__uuidof(IDXGISurface), (void**)&pdxgiSurface);
		m_d2dDeviceContext->CreateBitmapFromDxgiSurface(pdxgiSurface, &d2dBitmapProperties, &m_d2dRenderTargets[i]);
		if (pdxgiSurface) pdxgiSurface->Release();
	}
#pragma endregion
}

void CDevice::AddFont()
{
	std::vector<std::wstring> vec{ L"Arial", L"궁서체", L"바탕", L"맑은 고딕"};
	std::vector<float> vecSize{ 10, 20, 30, 40, 50 };
	for (const auto& name : vec)
	{
		for (float size : vecSize)
		{
			AddFontMachine(name, size);
		}
	}
}

void CDevice::AddColor()
{
	std::vector<std::wstring> vec{ L"Red", L"Green", L"Blue", L"Black", L"Yellow", L"White"};
	for (const auto& name : vec)
	{
		AddColorMachine(name);
	}
}

void CDevice::AddColorMachine(const std::wstring& name)
{
	ID2D1SolidColorBrush* d2dbrText = NULL;

	static const std::unordered_map<std::wstring, D2D1::ColorF::Enum> colorMap = 
	{
	  {L"Red", D2D1::ColorF::Red},
	  {L"Green", D2D1::ColorF::Green},
	  {L"Blue", D2D1::ColorF::Blue},
	  {L"Purple", D2D1::ColorF::Purple},
	  {L"Yellow", D2D1::ColorF::Yellow},
	  {L"Orange", D2D1::ColorF::Orange},
	  {L"Black", D2D1::ColorF::Black},
	  {L"White", D2D1::ColorF::White},
	  {L"Gray", D2D1::ColorF::Gray},
	};
	auto iter = colorMap.find(name);
	assert(iter != colorMap.end());

	HRESULT hr = m_d2dDeviceContext->CreateSolidColorBrush(D2D1::ColorF(iter->second, 1.0f), &d2dbrText);
	m_BrushMap[name] = d2dbrText;
}

void CDevice::AddFontMachine(const std::wstring& name, float size)
{
	IDWriteTextFormat*	 dwFont = NULL;

	HRESULT hResult = m_WriteFactory->CreateTextFormat(name.c_str(), NULL, DWRITE_FONT_WEIGHT_DEMI_BOLD, DWRITE_FONT_STYLE_ITALIC, DWRITE_FONT_STRETCH_NORMAL, size, L"en-US", &dwFont);
	hResult = dwFont->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
	hResult = dwFont->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
	m_FontMap[name + L"_" + std::to_wstring((int)size)] = dwFont;
}
