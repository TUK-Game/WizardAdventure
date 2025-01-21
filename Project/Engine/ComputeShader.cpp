#include "pch.h"
#include "ComputeShader.h"
#include "Device.h"

CComputeShader::CComputeShader()
	: CShader(EAsset_Type::ComputeShader)
	, m_ComputePipelineDesc{}
{
}

CComputeShader::~CComputeShader()
{
}

int CComputeShader::Init(const std::wstring& path, ShaderInfo info, const std::string& cs)
{
	m_Info = info;

	if (FAILED(CreateShader(path, cs, "cs_5_1", m_CsBlob, m_ComputePipelineDesc.CS)))
		return E_FAIL;

	m_ComputePipelineDesc.pRootSignature = COMPUTE_ROOT_SIGNATURE.Get();

	DEVICE->CreateComputePipelineState(&m_ComputePipelineDesc, IID_PPV_ARGS(&m_PipelineState));

	return S_OK;
}

void CComputeShader::Update()
{
	COMPUTE_CMD_LIST->SetPipelineState(m_PipelineState.Get());
}


