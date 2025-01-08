#include "pch.h"
#include "Mesh.h"
#include "Device.h"

CMesh::CMesh()
	: CAsset(EAsset_Type::Mesh)
	, m_VertexBufferView{}
	, m_VertexCount(0)
	, m_IndexBufferView{}
	, m_IndexCount(0)
{
}

CMesh::~CMesh()
{
}

int CMesh::Init(const std::vector<Vertex>& vecVertex, const std::vector<UINT>& vecIndex)
{
	if (FAILED(CreateVertexBuffer(vecVertex)))
		return E_FAIL;

	if (FAILED(CreateIndexBuffer(vecIndex)))
		return E_FAIL;

	return S_OK;
}

void CMesh::Render()
{
	CMD_LIST->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	CMD_LIST->IASetVertexBuffers(0, 1, &m_VertexBufferView); // Slot: (0~15)
	CMD_LIST->IASetIndexBuffer(&m_IndexBufferView);

	CDevice::GetInst()->GetTableDescHeap()->CommitTable();

	CMD_LIST->DrawIndexedInstanced(m_IndexCount, 1, 0, 0, 0);
}

int CMesh::CreateVertexBuffer(const std::vector<Vertex>& vecVertex)
{
	m_VertexCount = static_cast<UINT>(vecVertex.size());
	UINT bufferSize = m_VertexCount * sizeof(Vertex);

	D3D12_HEAP_PROPERTIES heapProperty = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	D3D12_RESOURCE_DESC desc = CD3DX12_RESOURCE_DESC::Buffer(bufferSize);

	if (FAILED(DEVICE->CreateCommittedResource(
		&heapProperty,
		D3D12_HEAP_FLAG_NONE,
		&desc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&m_VertexBuffer))))
	{
		return E_FAIL;
	}

	// Copy the triangle data to the vertex buffer.
	void* vertexDataBuffer = nullptr;
	CD3DX12_RANGE readRange(0, 0); // We do not intend to read from this resource on the CPU.
	m_VertexBuffer->Map(0, &readRange, &vertexDataBuffer);
	::memcpy(vertexDataBuffer, &vecVertex[0], bufferSize);
	m_VertexBuffer->Unmap(0, nullptr);

	// Initialize the vertex buffer view.
	m_VertexBufferView.BufferLocation = m_VertexBuffer->GetGPUVirtualAddress();
	m_VertexBufferView.StrideInBytes = sizeof(Vertex); // 정점 1개 크기
	m_VertexBufferView.SizeInBytes = bufferSize; // 버퍼의 크기	

	return S_OK;
}

int CMesh::CreateIndexBuffer(const std::vector<UINT>& vecIndex)
{
	m_IndexCount = static_cast<UINT>(vecIndex.size());
	UINT bufferSize = m_IndexCount * sizeof(UINT);

	D3D12_HEAP_PROPERTIES heapProperty = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	D3D12_RESOURCE_DESC desc = CD3DX12_RESOURCE_DESC::Buffer(bufferSize);

	if (FAILED(DEVICE->CreateCommittedResource(
		&heapProperty,
		D3D12_HEAP_FLAG_NONE,
		&desc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&m_IndexBuffer))))
	{
		return E_FAIL;
	}

	void* indexDataBuffer = nullptr;
	CD3DX12_RANGE readRange(0, 0); // We do not intend to read from this resource on the CPU.
	m_IndexBuffer->Map(0, &readRange, &indexDataBuffer);
	::memcpy(indexDataBuffer, &vecIndex[0], bufferSize);
	m_IndexBuffer->Unmap(0, nullptr);

	m_IndexBufferView.BufferLocation = m_IndexBuffer->GetGPUVirtualAddress();
	m_IndexBufferView.Format = DXGI_FORMAT_R32_UINT;
	m_IndexBufferView.SizeInBytes = bufferSize;

	return S_OK;
}
