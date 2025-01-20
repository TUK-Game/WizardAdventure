#include "pch.h"
#include "GraphicShader.h"
#include "Device.h"

CGraphicShader::CGraphicShader()
	: CShader(EAsset_Type::GraphicShader)
	, m_Topology(D3D12_PRIMITIVE_TOPOLOGY_TYPE::D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE)
	, m_GraphicsPipelineDesc{}
{
}

CGraphicShader::~CGraphicShader()
{
}

int CGraphicShader::Init(const std::wstring& path, const std::string& name, ShaderInfo info)
{
	m_Info = info;

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

	m_GraphicsPipelineDesc.InputLayout = { desc, _countof(desc) };
	m_GraphicsPipelineDesc.pRootSignature = GRAPHICS_ROOT_SIGNATURE.Get();

	m_GraphicsPipelineDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	m_GraphicsPipelineDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	m_GraphicsPipelineDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	m_GraphicsPipelineDesc.SampleMask = UINT_MAX;
	m_GraphicsPipelineDesc.PrimitiveTopologyType = info.topologyType;
	m_GraphicsPipelineDesc.NumRenderTargets = 1;
	m_GraphicsPipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	m_GraphicsPipelineDesc.SampleDesc.Count = 1;
	m_GraphicsPipelineDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;


	switch (info.shaderType)
	{
	case SHADER_TYPE::DEFERRED:
		m_GraphicsPipelineDesc.NumRenderTargets = RENDER_TARGET_G_BUFFER_GROUP_MEMBER_COUNT;
		m_GraphicsPipelineDesc.RTVFormats[0] = DXGI_FORMAT_R32G32B32A32_FLOAT; // POSITION
		m_GraphicsPipelineDesc.RTVFormats[1] = DXGI_FORMAT_R32G32B32A32_FLOAT; // NORMAL
		m_GraphicsPipelineDesc.RTVFormats[2] = DXGI_FORMAT_R8G8B8A8_UNORM; // COLOR
		break;
	case SHADER_TYPE::FORWARD:
		m_GraphicsPipelineDesc.NumRenderTargets = 1;
		m_GraphicsPipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
		break;
	}

	switch (info.rasterizerType)
	{
	case RASTERIZER_TYPE::CULL_BACK:
		m_GraphicsPipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
		m_GraphicsPipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;
		break;
	case RASTERIZER_TYPE::CULL_FRONT:
		m_GraphicsPipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
		m_GraphicsPipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_FRONT;
		break;
	case RASTERIZER_TYPE::CULL_NONE:
		m_GraphicsPipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
		m_GraphicsPipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
		break;
	case RASTERIZER_TYPE::WIREFRAME:
		m_GraphicsPipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_WIREFRAME;
		m_GraphicsPipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
		break;
	}

	switch (info.depthStencilType)
	{
	case DEPTH_STENCIL_TYPE::LESS:
		m_GraphicsPipelineDesc.DepthStencilState.DepthEnable = TRUE;
		m_GraphicsPipelineDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
		break;
	case DEPTH_STENCIL_TYPE::LESS_EQUAL:
		m_GraphicsPipelineDesc.DepthStencilState.DepthEnable = TRUE;
		m_GraphicsPipelineDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
		break;
	case DEPTH_STENCIL_TYPE::GREATER:
		m_GraphicsPipelineDesc.DepthStencilState.DepthEnable = TRUE;
		m_GraphicsPipelineDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_GREATER;
		break;
	case DEPTH_STENCIL_TYPE::GREATER_EQUAL:
		m_GraphicsPipelineDesc.DepthStencilState.DepthEnable = TRUE;
		m_GraphicsPipelineDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_GREATER_EQUAL;
		break;
	}

	DEVICE->CreateGraphicsPipelineState(&m_GraphicsPipelineDesc, IID_PPV_ARGS(&m_PipelineState));

	return S_OK;
}

void CGraphicShader::Update()
{
	GRAPHICS_CMD_LIST->SetPipelineState(m_PipelineState.Get());
}

int CGraphicShader::CreateVertexShader(const std::wstring& path, const std::string& name, const std::string& version)
{
	return CreateShader(path, name, version, m_VSBlob, m_GraphicsPipelineDesc.VS);
}

int CGraphicShader::CreatePixelShader(const std::wstring& path, const std::string& name, const std::string& version)
{
	return CreateShader(path, name, version, m_PSBlob, m_GraphicsPipelineDesc.PS);
}
