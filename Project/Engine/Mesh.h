#pragma once

#include "Asset.h"

class CMesh :
	public CAsset
{
public:
    CMesh();
	CMesh(const CMesh& other) = delete;
    ~CMesh();

public:
	int Init(const std::vector<Vertex>& vecVertex, const std::vector<UINT>& vecIndex);
	void Render();

private:
	int CreateVertexBuffer(const std::vector<Vertex>& vecVertex);
	int CreateIndexBuffer(const std::vector<UINT>& vecIndex);

public:
	virtual CMesh* Clone() override { return nullptr; }

private:
	ComPtr<ID3D12Resource>		m_VertexBuffer;
	D3D12_VERTEX_BUFFER_VIEW	m_VertexBufferView;
	UINT						m_VertexCount;

	ComPtr<ID3D12Resource>		m_IndexBuffer;
	D3D12_INDEX_BUFFER_VIEW		m_IndexBufferView;
	UINT						m_IndexCount;
};

