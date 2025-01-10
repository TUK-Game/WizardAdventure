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

void CMaterial::Binding()
{
	// CBV 업로드
	CONST_BUFFER(EConstantBuffer_Type::Material)->PushData(&m_Params, sizeof(m_Params));

	// SRV 업로드
	for (size_t i = 0; i < m_arrTexture.size(); i++)
	{
		if (m_arrTexture[i] == nullptr)
			continue;

		SRV_REGISTER reg = SRV_REGISTER(static_cast<char>(SRV_REGISTER::t0) + i);
		CDevice::GetInst()->GetTableDescHeap()->SetSRV(m_arrTexture[i]->GetCpuHandle(), reg);
	}

	// 파이프라인 세팅
	m_Shader->Update();
}
