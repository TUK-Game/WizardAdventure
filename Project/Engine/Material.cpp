#include "pch.h"
#include "Material.h"
#include "Device.h"

CMaterial::CMaterial()
	: CAsset(EAsset_Type::Material)
	, m_Params{}
{
}

CMaterial::~CMaterial()
{
}

void CMaterial::GraphicsBinding()
{
	// CBV 업로드
	CONST_BUFFER(EConstantBuffer_Type::Material)->PushGraphicsData(&m_Params, sizeof(m_Params));

	// SRV 업로드
	for (size_t i = 0; i < m_arrTexture.size(); i++)
	{
		if (m_arrTexture[i] == nullptr)
			continue;

		SRV_REGISTER reg = SRV_REGISTER(static_cast<char>(SRV_REGISTER::t0) + i);
		CDevice::GetInst()->GetGraphicsDescHeap()->SetSRV(m_arrTexture[i]->GetSRVCpuHandle(), reg);
	}

	// 파이프라인 세팅
	m_Shader->Update();
}

void CMaterial::ComputeBinding()
{
	// CBV 업로드
	CONST_BUFFER(EConstantBuffer_Type::Material)->PushGraphicsData(&m_Params, sizeof(m_Params));

	// SRV 업로드
	for (size_t i = 0; i < m_arrTexture.size(); i++)
	{
		if (m_arrTexture[i] == nullptr)
			continue;

		SRV_REGISTER reg = SRV_REGISTER(static_cast<char>(SRV_REGISTER::t0) + i);
		CDevice::GetInst()->GetComputeDescHeap()->SetSRV(m_arrTexture[i]->GetSRVCpuHandle(), reg);
	}

	// 파이프라인 세팅
	m_Shader->Update();
}

void CMaterial::Dispatch(UINT32 x, UINT32 y, UINT32 z)
{
	ComputeBinding();
	CDevice::GetInst()->GetComputeDescHeap()->CommitTable();

	COMPUTE_CMD_LIST->Dispatch(x, y, z);

	CDevice::GetInst()->GetComputeCmdQueue()->FlushResourceCommandQueue();
}
