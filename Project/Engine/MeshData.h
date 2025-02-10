#pragma once
#include "Asset.h"
#include "Ref.h"

class CMesh;
class CMaterial;
class CGameObject;

struct MeshRenderInfo
{
	CMesh*				mesh;
	std::vector<CMaterial*>	materials;
	Matrix				matrix;
};

class CMeshData
	: public CAsset
{
public:
	CMeshData();
	virtual ~CMeshData();

public:
	static CMeshData* LoadFromFBX(const std::wstring& path);

	virtual void Load(const std::wstring& path);
	virtual void Save(const std::wstring& path);

	std::vector<CGameObject*> Instantiate();

public:
	virtual CMeshData* Clone() override { return new CMeshData(*this); }

private:
	std::shared_ptr<CMesh>				m_Mesh;
	std::vector<std::shared_ptr<CMaterial>>	_materials;

	std::vector<MeshRenderInfo> _meshRenders;
};
