#pragma once

#include "RenderComponent.h"

// [32][32]
union InstanceID
{
	struct
	{
		UINT32 meshID;
		UINT32 materialID;
	};
	UINT64 id;
};

class CMeshRenderer :
	public CRenderComponent
{
public:
    CMeshRenderer();
    ~CMeshRenderer();

public:
    virtual void FinalUpdate() override;
    virtual void Render() override;
	void Render(std::shared_ptr<class CInstancingBuffer>& buffer);
	void RenderShadow();
	void RenderMap(std::shared_ptr<CInstancingBuffer>& buffer);
	void RenderMap();

    UINT64 GetInstanceID();

public:
    virtual CMeshRenderer* Clone() override { return new CMeshRenderer(*this); }
};

