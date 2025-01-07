#include "pch.h"
#include "DepthStencilBuffer.h"
#include "Device.h"

CDepthStencilBuffer::CDepthStencilBuffer()
	: m_DSVHandle{}
	, m_DSVFormat{}
{
}

CDepthStencilBuffer::~CDepthStencilBuffer()
{
}

int CDepthStencilBuffer::Init(const WindowInfo& window, DXGI_FORMAT dsvFormat)
{
	m_DSVFormat = dsvFormat;

	D3D12_HEAP_PROPERTIES heapProperty = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);

	D3D12_RESOURCE_DESC desc = CD3DX12_RESOURCE_DESC::Tex2D(m_DSVFormat, window.Width, window.Height);
	desc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

	D3D12_CLEAR_VALUE optimizedClearValue = CD3DX12_CLEAR_VALUE(m_DSVFormat, 1.0f, 0);

	if (FAILED(DEVICE->CreateCommittedResource(
		&heapProperty,
		D3D12_HEAP_FLAG_NONE,
		&desc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,
		&optimizedClearValue,
		IID_PPV_ARGS(&m_DSVBuffer))))
	{
		return E_FAIL;
	}

	D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
	heapDesc.NumDescriptors = 1;
	heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;

	if (FAILED(DEVICE->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&m_DSVHeap))))
		return E_FAIL;

	m_DSVHandle = m_DSVHeap->GetCPUDescriptorHandleForHeapStart();
	DEVICE->CreateDepthStencilView(m_DSVBuffer.Get(), nullptr, m_DSVHandle);

	return S_OK;
}
