#include "pch.h"
#include "RenderComponent.h"

CRenderComponent::CRenderComponent(EComponent_Type type)
	: CComponent(type)
{
}

CRenderComponent::~CRenderComponent()
{
}

void CRenderComponent::SetMaterial(CSharedPtr<CMaterial> material, UINT32 idx)
{
	if (m_Materials.size() <= static_cast<size_t>(idx))
		m_Materials.resize(static_cast<size_t>(idx + 1));

	m_Materials[idx] = material;
}
