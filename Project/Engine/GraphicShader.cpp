#include "pch.h"
#include "GraphicShader.h"
#include "Device.h"

CGraphicShader::CGraphicShader()
	: CShader(EAsset_Type::GraphicShader)
	, m_Topology(D3D12_PRIMITIVE_TOPOLOGY_TYPE::D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE)
{
}

CGraphicShader::~CGraphicShader()
{
}

int CGraphicShader::Init(const std::wstring& path, ShaderInfo info)
{
	if (FAILED(CreateVertexShader(path, "VS_Main", "vs_5_1")))
		return E_FAIL;

	if (FAILED(CreatePixelShader(path, "PS_Main", "ps_5_1")))
		return E_FAIL;

	D3D12_INPUT_ELEMENT_DESC desc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 32, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
	};

	m_PipelineDesc.InputLayout = { desc, _countof(desc) };
	m_PipelineDesc.pRootSignature = ROOT_SIGNATURE.Get();

	m_PipelineDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	m_PipelineDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	m_PipelineDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	m_PipelineDesc.SampleMask = UINT_MAX;
	m_PipelineDesc.PrimitiveTopologyType = m_Topology;
	m_PipelineDesc.NumRenderTargets = 1;
	m_PipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	m_PipelineDesc.SampleDesc.Count = 1;
	m_PipelineDesc.DSVFormat = CDevice::GetInst()->GetDepthStencilBuffer()->GetDSVFormat();

	switch (info.rasterizerType)
	{
	case RASTERIZER_TYPE::CULL_BACK:
		m_PipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
		m_PipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;
		break;
	case RASTERIZER_TYPE::CULL_FRONT:
		m_PipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
		m_PipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_FRONT;
		break;
	case RASTERIZER_TYPE::CULL_NONE:
		m_PipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
		m_PipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
		break;
	case RASTERIZER_TYPE::WIREFRAME:
		m_PipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_WIREFRAME;
		m_PipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
		break;
	}

	switch (info.depthStencilType)
	{
	case DEPTH_STENCIL_TYPE::LESS:
		m_PipelineDesc.DepthStencilState.DepthEnable = TRUE;
		m_PipelineDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
		break;
	case DEPTH_STENCIL_TYPE::LESS_EQUAL:
		m_PipelineDesc.DepthStencilState.DepthEnable = TRUE;
		m_PipelineDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
		break;
	case DEPTH_STENCIL_TYPE::GREATER:
		m_PipelineDesc.DepthStencilState.DepthEnable = TRUE;
		m_PipelineDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_GREATER;
		break;
	case DEPTH_STENCIL_TYPE::GREATER_EQUAL:
		m_PipelineDesc.DepthStencilState.DepthEnable = TRUE;
		m_PipelineDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_GREATER_EQUAL;
		break;
	}

	DEVICE->CreateGraphicsPipelineState(&m_PipelineDesc, IID_PPV_ARGS(&m_PipelineState));

	return S_OK;
}

void CGraphicShader::Update()
{
	CMD_LIST->SetPipelineState(m_PipelineState.Get());
}

int CGraphicShader::CreateShader(const std::wstring& path, const std::string& name, const std::string& version, ComPtr<ID3DBlob>& blob, D3D12_SHADER_BYTECODE& shaderByteCode)
{
	UINT compileFlag = 0;
#ifdef _DEBUG
	compileFlag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

	if (HRESULT HR = FAILED(::D3DCompileFromFile(path.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE
		, name.c_str(), version.c_str(), compileFlag, 0, &blob, &m_ErrBlob)))
	{
		::MessageBoxA(nullptr, "Shader Create Failed !", nullptr, MB_OK);
		return E_FAIL;
	}

	shaderByteCode = { blob->GetBufferPointer(), blob->GetBufferSize() };

	return S_OK;
}

int CGraphicShader::CreateVertexShader(const std::wstring& path, const std::string& name, const std::string& version)
{
	return CreateShader(path, name, version, m_VSBlob, m_PipelineDesc.VS);
}

int CGraphicShader::CreatePixelShader(const std::wstring& path, const std::string& name, const std::string& version)
{
	return CreateShader(path, name, version, m_PSBlob, m_PipelineDesc.PS);
}
