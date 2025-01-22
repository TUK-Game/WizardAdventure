#pragma once

struct InstancingParams
{
	Matrix matWorld;
	Matrix matWV;
	Matrix matWVP;
};

class CInstancingBuffer
{
public:
	CInstancingBuffer();
	~CInstancingBuffer();

	void Init(UINT32 maxCount = 10);

	void Clear();
	void AddData(InstancingParams& params);
	void PushData();

public:
	UINT32						GetCount() { return static_cast<UINT32>(m_Data.size()); }
	ComPtr<ID3D12Resource>		GetBuffer() { return m_Buffer; }
	D3D12_VERTEX_BUFFER_VIEW	GetBufferView() { return m_BufferView; }

	void	SetID(UINT64 instanceId) { m_InstanceId = instanceId; }
	UINT64	GetID() { return m_InstanceId; }

private:
	UINT64							m_InstanceId = 0;
	ComPtr<ID3D12Resource>			m_Buffer;
	D3D12_VERTEX_BUFFER_VIEW		m_BufferView;

	UINT32							m_MaxCount = 0;
	std::vector<InstancingParams>	m_Data;
};