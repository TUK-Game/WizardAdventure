#include "pch.h"
#include "Component.h"

CComponent::CComponent(EComponent_Type type)
	: m_Type(type)
	, m_Owner(nullptr)
{
}

CComponent::~CComponent()
{
}
