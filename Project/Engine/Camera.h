#pragma once

#include "Component.h"

class CCamera :
	public CComponent
{
	friend class CRenderManager;

public:
	CCamera();
	~CCamera();

public:
	virtual void FinalUpdate() override;
	void Render();

public:
	virtual CCamera* Clone() override { return new CCamera(*this); }

private:
	//TODO: LayerCheck
};

