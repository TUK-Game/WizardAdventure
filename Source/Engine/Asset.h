#pragma once

#include "Ref.h"

class CAsset :
	public CRef
{
	friend class CAssetManager;

public:
	CAsset(EAsset_Type type);
	~CAsset();

public:
	EAsset_Type GetType() const	{ return m_Type; }

public:
	virtual CAsset* Clone() = 0;

private:
	EAsset_Type		m_Type;
	std::wstring	m_Key;
	std::wstring	m_RelativePath;
};

