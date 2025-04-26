#include "pch.h"
#include "CommandQueue.h"
#include "SwapChain.h"
#include "Device.h"
#include "LevelManager.h"
#include "Level.h"
#include "WidgetWindow.h"

// ************************
// GraphicsCommandQueue
// ************************

CGraphicsCommandQueue::CGraphicsCommandQueue()
	: m_FenceValue(0)
	, m_FenceEvent(INVALID_HANDLE_VALUE)
{
}

CGraphicsCommandQueue::~CGraphicsCommandQueue()
{
	::CloseHandle(m_FenceEvent);
}

int CGraphicsCommandQueue::Init(ComPtr<ID3D12Device> device, std::shared_ptr<CSwapChain> swapChain)
{
	m_SwapChain = swapChain;

	D3D12_COMMAND_QUEUE_DESC queueDesc = {};
	queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;

	if (FAILED(device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&m_GraphicsCmdQueue))))
		return E_FAIL;

	if (FAILED(device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_CmdAlloc))))
		return E_FAIL;

	if (FAILED(device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_CmdAlloc.Get(), nullptr, IID_PPV_ARGS(&m_CmdList))))
		return E_FAIL;

	m_CmdList->Close();

	if (FAILED(device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_ResCmdAlloc))))
		return E_FAIL;

	if (FAILED(device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_ResCmdAlloc.Get(), nullptr, IID_PPV_ARGS(&m_ResCmdList))))
		return E_FAIL;

	// CreateFence
	if (FAILED(device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_Fence))))
		return E_FAIL;

	m_FenceEvent = ::CreateEvent(nullptr, FALSE, FALSE, nullptr);

	return S_OK;
}

void CGraphicsCommandQueue::WaitSync()
{
	// Advance the fence value to mark commands up to this fence point.
	m_FenceValue++;

	// Add an instruction to the command queue to set a new fence point.  Because we 
	// are on the GPU timeline, the new fence point won't be set until the GPU finishes
	// processing all the commands prior to this Signal().
	m_GraphicsCmdQueue->Signal(m_Fence.Get(), m_FenceValue);

	// Wait until the GPU has completed commands up to this fence point.
	if (m_Fence->GetCompletedValue() < m_FenceValue)
	{
		// Fire event when GPU hits current fence.  
		m_Fence->SetEventOnCompletion(m_FenceValue, m_FenceEvent);

		// Wait until the GPU hits current fence event is fired.
		::WaitForSingleObject(m_FenceEvent, INFINITE);
	}
}


void CGraphicsCommandQueue::RenderBegin()
{
	m_CmdAlloc->Reset();
	m_CmdList->Reset(m_CmdAlloc.Get(), nullptr);

	INT8 backIndex = m_SwapChain->GetBackBufferIndex();

	D3D12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
		CDevice::GetInst()->GetRenderTargetGroup(RENDER_TARGET_GROUP_TYPE::SWAP_CHAIN)->GetRTTexture(backIndex)->GetTex2D().Get(),
		D3D12_RESOURCE_STATE_PRESENT,
		D3D12_RESOURCE_STATE_RENDER_TARGET);

	m_CmdList->SetGraphicsRootSignature(GRAPHICS_ROOT_SIGNATURE.Get());

	CONST_BUFFER(EConstantBuffer_Type::Transform)->Clear();
	CONST_BUFFER(EConstantBuffer_Type::Material)->Clear();

	CDevice::GetInst()->GetGraphicsDescHeap()->Clear();

	ID3D12DescriptorHeap* descHeap = CDevice::GetInst()->GetGraphicsDescHeap()->GetDescriptorHeap().Get();
	m_CmdList->SetDescriptorHeaps(1, &descHeap);

	m_CmdList->ResourceBarrier(1, &barrier);

}

void CGraphicsCommandQueue::RenderEnd()
{
	INT8 backIndex = m_SwapChain->GetBackBufferIndex();

	D3D12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
		CDevice::GetInst()->GetRenderTargetGroup(RENDER_TARGET_GROUP_TYPE::SWAP_CHAIN)->GetRTTexture(backIndex)->GetTex2D().Get(),
		D3D12_RESOURCE_STATE_RENDER_TARGET,
		D3D12_RESOURCE_STATE_PRESENT);

	m_CmdList->ResourceBarrier(1, &barrier);
	m_CmdList->Close();

	// 커맨드 리스트 수행
	ID3D12CommandList* cmdListArr[] = { m_CmdList.Get() };
	m_GraphicsCmdQueue->ExecuteCommandLists(_countof(cmdListArr), cmdListArr);

	// 텍스트 렌더링
	CLevel* level = CLevelManager::GetInst()->GetCurrentLevel();
	if (level && (EWIDGETWINDOW_TYPE::TEXT_WINDOW == level->GetWidgetWindowType() || EWIDGETWINDOW_TYPE::END == level->GetWidgetWindowType()))
	{
		CDevice::GetInst()->m_d2dDeviceContext->SetTarget(CDevice::GetInst()->m_d2dRenderTargets[CDevice::GetInst()->GetSwapChain()->GetBackBufferIndex()].Get());
		ID3D11Resource* ppd3dResources[] = { CDevice::GetInst()->m_d3d11WrappedBackBuffers[CDevice::GetInst()->GetSwapChain()->GetBackBufferIndex()].Get() };
		CDevice::GetInst()->m_d3d11On12Device->AcquireWrappedResources(ppd3dResources, _countof(ppd3dResources));

		CDevice::GetInst()->m_d2dDeviceContext->BeginDraw();

		auto window = level->GetWidgetWindow(EWIDGETWINDOW_TYPE::TEXT_WINDOW);
		if (window)
		{
			window->Render();
		}
		CDevice::GetInst()->m_d2dDeviceContext->EndDraw();
		CDevice::GetInst()->m_d3d11On12Device->ReleaseWrappedResources(ppd3dResources, _countof(ppd3dResources));
		CDevice::GetInst()->m_DeviceContext->Flush();
	}
	m_SwapChain->Present();

	// Wait until frame commands are complete.  This waiting is inefficient and is
	// done for simplicity.  Later we will show how to organize our rendering code
	// so we do not have to wait per frame.
	WaitSync();

	m_SwapChain->SwapIndex();
}

void CGraphicsCommandQueue::FlushResourceCommandQueue()
{
	m_ResCmdList->Close();

	ID3D12CommandList* cmdListArr[] = { m_ResCmdList.Get() };
	m_GraphicsCmdQueue->ExecuteCommandLists(_countof(cmdListArr), cmdListArr);

	WaitSync();

	m_ResCmdAlloc->Reset();
	m_ResCmdList->Reset(m_ResCmdAlloc.Get(), nullptr);
}

// ************************
// ComputeCommandQueue
// ************************

CComputeCommandQueue::CComputeCommandQueue()
	: m_FenceValue(0)
	, m_FenceEvent(INVALID_HANDLE_VALUE)
{
}

CComputeCommandQueue::~CComputeCommandQueue()
{
	::CloseHandle(m_FenceEvent);
}

int CComputeCommandQueue::Init(ComPtr<ID3D12Device> device)
{
	D3D12_COMMAND_QUEUE_DESC computeQueueDesc = {};
	computeQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_COMPUTE;
	computeQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;

	if (FAILED(device->CreateCommandQueue(&computeQueueDesc, IID_PPV_ARGS(&m_CmdQueue))))
		return E_FAIL;

	if (FAILED(device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_COMPUTE, IID_PPV_ARGS(&m_CmdAlloc))))
		return E_FAIL;

	if (FAILED(device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_COMPUTE, m_CmdAlloc.Get(), nullptr, IID_PPV_ARGS(&m_CmdList))))
		return E_FAIL;

	if (FAILED(device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_Fence))))
		return E_FAIL;

	// CreateFence
	// - CPU와 GPU의 동기화 수단으로 쓰인다
	if (FAILED(device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_Fence))))
		return E_FAIL;

	m_FenceEvent = ::CreateEvent(nullptr, FALSE, FALSE, nullptr);
}

void CComputeCommandQueue::WaitSync()
{
	// Advance the fence value to mark commands up to this fence point.
	m_FenceValue++;

	// Add an instruction to the command queue to set a new fence point.  Because we 
	// are on the GPU timeline, the new fence point won't be set until the GPU finishes
	// processing all the commands prior to this Signal().
	m_CmdQueue->Signal(m_Fence.Get(), m_FenceValue);

	// Wait until the GPU has completed commands up to this fence point.
	if (m_Fence->GetCompletedValue() < m_FenceValue)
	{
		// Fire event when GPU hits current fence.  
		m_Fence->SetEventOnCompletion(m_FenceValue, m_FenceEvent);

		// Wait until the GPU hits current fence event is fired.
		::WaitForSingleObject(m_FenceEvent, INFINITE);
	}
}

void CComputeCommandQueue::FlushResourceCommandQueue()
{
	m_CmdList->Close();

	ID3D12CommandList* cmdListArr[] = { m_CmdList.Get() };
	m_CmdQueue->ExecuteCommandLists(_countof(cmdListArr), cmdListArr);

	WaitSync();

	m_CmdAlloc->Reset();
	m_CmdList->Reset(m_CmdAlloc.Get(), nullptr);

	COMPUTE_CMD_LIST->SetComputeRootSignature(COMPUTE_ROOT_SIGNATURE.Get());
}
