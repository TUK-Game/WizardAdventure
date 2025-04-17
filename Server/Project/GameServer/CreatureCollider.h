#pragma once
#include "BoxCollider.h"	

class CCreatureCollider :
	public CBoxCollider
{
public:
	virtual void Update() override;
	virtual void SetBoxInfo(XMFLOAT3 centerPos, XMFLOAT3 size, XMFLOAT3 rotation, XMFLOAT3 offset = XMFLOAT3(0.f, 0.f, 0.f));
};

