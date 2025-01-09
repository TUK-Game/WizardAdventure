#include "pch.h"
#include "CommandQueue.h"
#include "SwapChain.h"
#include "Device.h"

CCommandQueue::CCommandQueue()
	: m_FenceValue(0)
	, m_FenceEvent(INVALID_HANDLE_VALUE)
{
}

CCommandQueue::~CCommandQueue()
{
	::CloseHandle(m_FenceEvent);
}

int CCommandQueue::Init(ComPtr<ID3D12Device> device, std::shared_ptr<CSwapChain> swapChain)
{
	m_SwapChain = swapChain;

	D3D12_COMMAND_QUEUE_DESC queueDesc = {};
	queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;

	if (FAILED(device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&m_CmdQueue))))
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

void CCommandQueue::WaitSync()
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


void CCommandQueue::RenderBegin(const D3D12_VIEWPORT* vp, const D3D12_RECT* rect)
{
	m_CmdAlloc->Reset();
	m_CmdList->Reset(m_CmdAlloc.Get(), nullptr);

	D3D12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
		m_SwapChain->GetBackRTVBuffer().Get(),
		D3D12_RESOURCE_STATE_PRESENT,
		D3D12_RESOURCE_STATE_RENDER_TARGET);

	m_CmdList->SetGraphicsRootSignature(ROOT_SIGNATURE.Get());

	CONST_BUFFER(EConstantBuffer_Type::Transform)->Clear();
	//CONST_BUFFER(EConstantBuffer_Type::Material)->Clear();

	CDevice::GetInst()->GetTableDescHeap()->Clear();

	ID3D12DescriptorHeap* descHeap = CDevice::GetInst()->GetTableDescHeap()->GetDescriptorHeap().Get();
	m_CmdList->SetDescriptorHeaps(1, &descHeap);

	m_CmdList->ResourceBarrier(1, &barrier);

	// Set the viewport and scissor rect.  This needs to be reset whenever the command list is reset.
	m_CmdList->RSSetViewports(1, vp);
	m_CmdList->RSSetScissorRects(1, rect);

	// Specify the buffers we are going to render to.
	D3D12_CPU_DESCRIPTOR_HANDLE backBufferView = m_SwapChain->GetBackRTV();
	m_CmdList->ClearRenderTargetView(backBufferView, Colors::Blue, 0, nullptr);

	D3D12_CPU_DESCRIPTOR_HANDLE depthStencilView = CDevice::GetInst()->GetDepthStencilBuffer()->GetDSVCpuHandle();
	m_CmdList->OMSetRenderTargets(1, &backBufferView, FALSE, &depthStencilView);

	m_CmdList->ClearDepthStencilView(depthStencilView, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
}

void CCommandQueue::RenderEnd()
{
	D3D12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
		m_SwapChain->GetBackRTVBuffer().Get(),
		D3D12_RESOURCE_STATE_RENDER_TARGET,
		D3D12_RESOURCE_STATE_PRESENT);

	m_CmdList->ResourceBarrier(1, &barrier);
	m_CmdList->Close();

	// 커맨드 리스트 수행
	ID3D12CommandList* cmdListArr[] = { m_CmdList.Get() };
	m_CmdQueue->ExecuteCommandLists(_countof(cmdListArr), cmdListArr);

	m_SwapChain->Present();

	// Wait until frame commands are complete.  This waiting is inefficient and is
	// done for simplicity.  Later we will show how to organize our rendering code
	// so we do not have to wait per frame.
	WaitSync();

	m_SwapChain->SwapIndex();
}

void CCommandQueue::FlushResourceCommandQueue()
{
	m_ResCmdList->Close();

	ID3D12CommandList* cmdListArr[] = { m_ResCmdList.Get() };
	m_CmdQueue->ExecuteCommandLists(_countof(cmdListArr), cmdListArr);

	WaitSync();

	m_ResCmdAlloc->Reset();
	m_ResCmdList->Reset(m_ResCmdAlloc.Get(), nullptr);
}