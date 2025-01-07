#include "pch.h"
#include "SwapChain.h"

CSwapChain::CSwapChain()
	: m_RTVHandle{}
	, m_BackBufferIndex(0)
{
}

CSwapChain::~CSwapChain()
{
}

int CSwapChain::Init(const WindowInfo& info, ComPtr<ID3D12Device> device, ComPtr<IDXGIFactory> dxgi, ComPtr<ID3D12CommandQueue> cmdQueue)
{
	if (FAILED(CreateSwapChain(info, dxgi, cmdQueue)))
		return E_FAIL;

	if (FAILED(CreateRTV(device)))
		return E_FAIL;

	return S_OK;
}

void CSwapChain::Present()
{
	// Present the frame.
	m_SwapChain->Present(0, 0);
}

void CSwapChain::SwapIndex()
{
	m_BackBufferIndex = (m_BackBufferIndex + 1) % SWAP_CHAIN_BUFFER_COUNT;
}

int CSwapChain::CreateSwapChain(const WindowInfo& info, ComPtr<IDXGIFactory> dxgi, ComPtr<ID3D12CommandQueue> cmdQueue)
{
	// ������ ���� ���� ������
	m_SwapChain.Reset();

	DXGI_SWAP_CHAIN_DESC sd;
	sd.BufferDesc.Width = static_cast<UINT>(info.Width); // ������ �ػ� �ʺ�
	sd.BufferDesc.Height = static_cast<UINT>(info.Height); // ������ �ػ� ����
	sd.BufferDesc.RefreshRate.Numerator = 60; // ȭ�� ���� ����
	sd.BufferDesc.RefreshRate.Denominator = 1; // ȭ�� ���� ����
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // ������ ���÷��� ����
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	sd.SampleDesc.Count = 1; // ��Ƽ ���ø� OFF
	sd.SampleDesc.Quality = 0;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // �ĸ� ���ۿ� �������� �� 
	sd.BufferCount = SWAP_CHAIN_BUFFER_COUNT; // ���� + �ĸ� ����
	sd.OutputWindow = info.hWnd;
	sd.Windowed = info.bWindowed;
	sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD; // ���� �ĸ� ���� ��ü �� ���� ������ ���� ����
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	if (FAILED(dxgi->CreateSwapChain(cmdQueue.Get(), &sd, &m_SwapChain)))
		return E_FAIL;

	for (int i = 0; i < SWAP_CHAIN_BUFFER_COUNT; i++)
		m_SwapChain->GetBuffer(i, IID_PPV_ARGS(&m_RTVBuffer[i]));

	return S_OK;
}

int CSwapChain::CreateRTV(ComPtr<ID3D12Device> device)
{
	// Descriptor (DX12) = View (~DX11)
	// [������ ��]���� RTV ����
	// DX11�� RTV(RenderTargetView), DSV(DepthStencilView), 
	// CBV(ConstantBufferView), SRV(ShaderResourceView), UAV(UnorderedAccessView)�� ����!

	int rtvHeapSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	D3D12_DESCRIPTOR_HEAP_DESC rtvDesc;
	rtvDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	rtvDesc.NumDescriptors = SWAP_CHAIN_BUFFER_COUNT;
	rtvDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	rtvDesc.NodeMask = 0;

	// ���� ������ �����ͳ��� �迭�� ����
	// RTV ��� : [ ] [ ]
	if (FAILED(device->CreateDescriptorHeap(&rtvDesc, IID_PPV_ARGS(&m_RTVHeap))))
		return E_FAIL;

	D3D12_CPU_DESCRIPTOR_HANDLE rtvHeapBegin = m_RTVHeap->GetCPUDescriptorHandleForHeapStart();

	for (int i = 0; i < SWAP_CHAIN_BUFFER_COUNT; i++)
	{
		m_RTVHandle[i] = CD3DX12_CPU_DESCRIPTOR_HANDLE(rtvHeapBegin, i * rtvHeapSize);
		device->CreateRenderTargetView(m_RTVBuffer[i].Get(), nullptr, m_RTVHandle[i]);
	}

	return S_OK;
}