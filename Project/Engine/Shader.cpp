#include "pch.h"
#include "Shader.h"

CShader::CShader(EAsset_Type type)
	: CAsset(type)
{
}

CShader::~CShader()
{
}

int CShader::CreateShader(const std::wstring& path, const std::string& name, const std::string& version, ComPtr<ID3DBlob>& blob, D3D12_SHADER_BYTECODE& shaderByteCode)
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
