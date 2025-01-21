#pragma once

enum class EConstantBuffer_Type : unsigned char
{
	Global, 
	Transform,
	Material,

	END
};

enum
{
	CONSTANT_BUFFER_COUNT = static_cast<unsigned char>(EConstantBuffer_Type::END)
};

class CConstantBuffer
{
public:
	CConstantBuffer();
	~CConstantBuffer();
	
public:
	D3D12_GPU_VIRTUAL_ADDRESS GetGpuVirtualAddress(UINT index);
	D3D12_CPU_DESCRIPTOR_HANDLE GetSRVCpuHandle(UINT index);

public:
	int Init(CBV_REGISTER reg, UINT size, UINT count);

	void Clear();
	void PushGraphicsData(void* buffer, UINT size);
	void PushComputeData(void* buffer, UINT size);

	void SetGlobalData(void* buffer, UINT size);

private:
	int CreateBuffer();
	int CreateView();

private:
	ComPtr<ID3D12Resource>	m_CBVBuffer;
	BYTE*					m_MappedBuffer;
	UINT					m_ElementSize;
	UINT					m_ElementCount;

	ComPtr<ID3D12DescriptorHeap>		m_CBVHeap;
	D3D12_CPU_DESCRIPTOR_HANDLE			m_CPUHandleBegin;
	UINT								m_HandleIncrementSize;

	UINT					m_CurrentIndex;

	CBV_REGISTER			m_Register;
};

