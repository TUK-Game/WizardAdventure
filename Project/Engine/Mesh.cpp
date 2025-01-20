#include "pch.h"
#include "Mesh.h"
#include "Device.h"
#include "FBXLoader.h"

CMesh::CMesh()
	: CAsset(EAsset_Type::Mesh)
	, m_VertexBufferView{}
	, m_VertexCount(0)
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

void CMesh::Render(UINT32 idx)
{
	GRAPHICS_CMD_LIST->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	GRAPHICS_CMD_LIST->IASetVertexBuffers(0, 1, &m_VertexBufferView); // Slot: (0~15)
	GRAPHICS_CMD_LIST->IASetIndexBuffer(&m_VecIndexInfo[idx].bufferView);

	CDevice::GetInst()->GetGraphicsDescHeap()->CommitTable();

	GRAPHICS_CMD_LIST->DrawIndexedInstanced(m_VecIndexInfo[idx].count, 1, 0, 0, 0);
}

CMesh* CMesh::CreateFromFBX(const FbxMeshInfo* meshInfo)
{
	CMesh* mesh = new CMesh;
	mesh->CreateVertexBuffer(meshInfo->vertices);
	mesh->SetMeshSize(meshInfo->maxPos);
	mesh->SetName(meshInfo->name);

	for (const std::vector<UINT32>& buffer : meshInfo->indices)
	{
		if (buffer.empty())
		{
			// FBX 파일이 이상하다. IndexBuffer가 없으면 에러 나니까 임시 처리
			std::vector<UINT32> defaultBuffer{ 0 };
			mesh->CreateIndexBuffer(defaultBuffer);
		}
		else
		{
			mesh->CreateIndexBuffer(buffer);
		}
	}

	return mesh;
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

int CMesh::CreateIndexBuffer(const std::vector<UINT>& buffer)
{
	UINT32 indexCount = static_cast<UINT32>(buffer.size());
	UINT32 bufferSize = indexCount * sizeof(UINT32);

	D3D12_HEAP_PROPERTIES heapProperty = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	D3D12_RESOURCE_DESC desc = CD3DX12_RESOURCE_DESC::Buffer(bufferSize);

	ComPtr<ID3D12Resource> indexBuffer;
	DEVICE->CreateCommittedResource(
		&heapProperty,
		D3D12_HEAP_FLAG_NONE,
		&desc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&indexBuffer));

	void* indexDataBuffer = nullptr;
	CD3DX12_RANGE readRange(0, 0);
	indexBuffer->Map(0, &readRange, &indexDataBuffer);
	::memcpy(indexDataBuffer, &buffer[0], bufferSize);
	indexBuffer->Unmap(0, nullptr);

	D3D12_INDEX_BUFFER_VIEW	indexBufferView;
	indexBufferView.BufferLocation = indexBuffer->GetGPUVirtualAddress();
	indexBufferView.Format = DXGI_FORMAT_R32_UINT;
	indexBufferView.SizeInBytes = bufferSize;

	IndexBufferInfo info =
	{
		indexBuffer,
		indexBufferView,
		DXGI_FORMAT_R32_UINT,
		indexCount
	};

	m_VecIndexInfo.push_back(info);

	return S_OK;
}
