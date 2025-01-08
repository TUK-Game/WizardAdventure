#include "pch.h"
#include "Shader.h"

CShader::CShader(EAsset_Type type)
	: CAsset(type)
	, m_PipelineDesc{}
{
}

CShader::~CShader()
{
}
