#pragma once

#include "Asset.h"

struct IndexBufferInfo
{
	ComPtr<ID3D12Resource>		buffer;
	D3D12_INDEX_BUFFER_VIEW		bufferView;
	DXGI_FORMAT					format;
	UINT32						count;
};

class CMesh :
	public CAsset
{
public:
    CMesh();
	CMesh(const CMesh& other) = delete;
    ~CMesh();

public:
	int Init(const std::vector<Vertex>& vecVertex, const std::vector<UINT>& vecIndex);
	void Render(UINT32 instanceCount = 1, UINT32 idx = 0);

	static CMesh* CreateFromFBX(const struct FbxMeshInfo* meshInfo);
private:
	int CreateVertexBuffer(const std::vector<Vertex>& vecVertex);
	int CreateIndexBuffer(const std::vector<UINT>& buffer);

public:
	UINT32 GetSubsetCount() { return static_cast<UINT32>(m_VecIndexInfo.size()); }
	void SetMeshSize(Vec3 size) { m_MeshSize = size; }
	Vec3 GetMeshSize() { return m_MeshSize; }
public:
	virtual CMesh* Clone() override { return nullptr; }

private:
	ComPtr<ID3D12Resource>		m_VertexBuffer;
	D3D12_VERTEX_BUFFER_VIEW	m_VertexBufferView;
	UINT						m_VertexCount;

	std::vector<IndexBufferInfo>		m_VecIndexInfo;

	Vec3 m_MeshSize;
};

