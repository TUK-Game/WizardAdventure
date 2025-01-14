#include "pch.h"
#include "Device.h"
#include "Engine.h"

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

	m_CmdQueue = std::make_shared<CCommandQueue>();
	m_SwapChain = std::make_shared<CSwapChain>();
	m_RootSignature = std::make_shared<CRootSignature>();
	m_TableDescHeap = std::make_shared<CTableDescriptorHeap>();
	m_DepthStencilBuffer = std::make_shared<CDepthStencilBuffer>();

	if (FAILED(m_CmdQueue->Init(m_Device, m_SwapChain)))
		return E_FAIL;

	if (FAILED(m_SwapChain->Init(CEngine::GetInst()->GetWindowInfo(), m_Device, m_DXGI, m_CmdQueue->GetCmdQueue())))
		return E_FAIL;

	if (FAILED(m_RootSignature->Init()))
		return E_FAIL;

	if (FAILED(m_TableDescHeap->Init(5)))
		return E_FAIL;

	if (FAILED(m_DepthStencilBuffer->Init(CEngine::GetInst()->GetWindowInfo())))
		return E_FAIL;

	// 사용할 수에 맞게 레지스터 개수 생성 ex) 렌더링 오브젝트 2개면 각 2개씩
	CreateConstantBuffer(CBV_REGISTER::b0, sizeof(TransformParams), 5);
	CreateConstantBuffer(CBV_REGISTER::b1, sizeof(MaterialParams), 5);

	return S_OK;
}

void CDevice::RenderBegin()	
{
	m_CmdQueue->RenderBegin(&m_Viewport, &m_ScissorRect);
}

void CDevice::RenderEnd()
{
	m_CmdQueue->RenderEnd();
}

void CDevice::CreateConstantBuffer(CBV_REGISTER reg, UINT bufferSize, UINT count)
{
	unsigned char typeInt = static_cast<unsigned char>(reg);
	assert(m_ConstantBuffers.size() == typeInt);

	std::shared_ptr<CConstantBuffer> buffer = std::make_shared<CConstantBuffer>();
	buffer->Init(reg, bufferSize, count);
	m_ConstantBuffers.push_back(buffer);
}
