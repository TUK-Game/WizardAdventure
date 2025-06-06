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
	Vec4 translation;
	Vec4 rotation;
	Vec4 scale;
	Vec3 boundingBoxMax;
	Vec3 boundingBoxMin;
	Vec4 centerPos;
};

class CMeshData
	: public CAsset
{
public:
	CMeshData();
	virtual ~CMeshData();

public:
	std::vector<MeshRenderInfo>& GetMeshrenderInfos() { return _meshRenders; }
	MeshRenderInfo& GetMeshrenderInfo(int idx) { return _meshRenders[idx]; }

public:
	static CMeshData* LoadFromJHD(const std::wstring& path, const std::wstring& texturePath);

	virtual void Load(const std::wstring& path);
	virtual void Save(const std::wstring& path);

	std::vector<CGameObject*> Instantiate(ECollision_Channel channel);
	std::vector<CGameObject*> InstantiateWalls();

public:
	virtual CMeshData* Clone() override { return new CMeshData(*this); }

private:
	std::shared_ptr<CMesh>				m_Mesh;
	std::vector<std::shared_ptr<CMaterial>>	_materials;

	std::vector<MeshRenderInfo> _meshRenders;
};
