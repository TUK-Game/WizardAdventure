#pragma once
#include "Asset.h"
struct JHDMaterialInfo
{
	Vec4			diffuse;
	Vec4			ambient;
	Vec4			specular;
	std::wstring			name;
	std::wstring			diffuseTexName;
	std::wstring			normalTexName;
	std::wstring			specularTexName;
};


struct JHDMeshInfo : public CAsset
{
	JHDMeshInfo() : CAsset(EAsset_Type::JHDMeshData) {}
	~JHDMeshInfo() {}

	std::wstring							name;
	std::vector<Vertex>						vertices;
	std::vector<std::vector<UINT32>>		indices;
	std::vector<JHDMaterialInfo>			materials;
	//std::vector<BoneWeight>					boneWeights; // »À °¡ÁßÄ¡
	//bool									hasAnimation;
	//Vec3 maxPos{};
	//Vec3 minPos{};
	FbxAMatrix matrix;
	Vec4 translate;
	Vec3 scale;
	Vec4 rotation;
	Vec4 centerPos;
	Vec3 BoundingBoxMax;
	Vec3 BoundingBoxMin;

	virtual JHDMeshInfo* Clone() override { return new JHDMeshInfo(*this); }

};


class CJHDLoader
{
public:
	INT32 GetMeshCount() { return static_cast<INT32>(m_Meshes.size()); }
	const JHDMeshInfo& GetMesh(INT32 idx) { return m_Meshes[idx]; }

public:
	void LoadFile(const char* filename, const std::wstring& textureFilename = L"");
	void LoadMaterial(std::ifstream& file, bool bCopy);

	void CreateTextures();
	void CreateMaterials();

private:
	std::vector<JHDMeshInfo>					m_Meshes;
	std::wstring m_ResourceDirectory;
};

