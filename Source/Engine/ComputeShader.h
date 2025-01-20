#pragma once
#include "Shader.h"
class CComputeShader :
	public CShader
{
public:
	CComputeShader();
	virtual ~CComputeShader();

	int Init(const std::wstring& path, const std::string& name, ShaderInfo info = ShaderInfo());
	void Update();

private:
	ComPtr<ID3DBlob>					m_CsBlob;
	D3D12_COMPUTE_PIPELINE_STATE_DESC   m_ComputePipelineDesc;

};

