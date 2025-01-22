#include "pch.h"
#include "Asset.h"

CAsset::CAsset(EAsset_Type type)
	: m_Type(type)
{
	static UINT32 idGenerator = 1;
	m_ID = idGenerator++;
}

CAsset::~CAsset()
{
}
