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

struct BoneWeight
{
	using Pair = std::pair<INT32, double>;
	std::vector<Pair> boneWeights;
};


struct JHDMeshInfo : public CAsset
{
	JHDMeshInfo() : CAsset(EAsset_Type::JHDMeshData) {}
	~JHDMeshInfo() {}

	std::wstring							name;
	std::vector<Vertex>						vertices;
	std::vector<std::vector<UINT32>>		indices;
	std::vector<JHDMaterialInfo>			materials;
	std::vector<BoneWeight>					boneWeights; // »À °¡ÁßÄ¡
	bool									hasAnimation = false;
	Matrix matrix;
	Vec4 translate;
	Vec4 rotation;
	Vec4 scale;
	Vec4 centerPos;
	Vec3 BoundingBoxMax;
	Vec3 BoundingBoxMin;

	virtual JHDMeshInfo* Clone() override { return new JHDMeshInfo(*this); }

};

struct FbxKeyFrameInfo
{
	Matrix  matTransform;
	double		time;
};

struct FbxBoneInfo
{
	std::wstring			boneName;
	INT32					parentIndex;
	Matrix				matOffset;
};

struct FbxAnimClipInfo
{
	std::wstring			name;
	double					startTime;
	double					endTime;
	INT32					startFrame;
	INT32					endFrame;
	std::vector<std::vector<std::vector<FbxKeyFrameInfo>>>	keyFrames;
};

class CJHDLoader
{
public:
	INT32 GetMeshCount() { return static_cast<INT32>(m_Meshes.size()); }
	const JHDMeshInfo& GetMesh(INT32 idx) { return m_Meshes[idx]; }
	std::vector<std::vector<std::shared_ptr<FbxBoneInfo>>>& GetBones() { return m_Bones; }
	std::vector<std::shared_ptr<FbxAnimClipInfo>>& GetAnimClip() { return m_AnimClips; }
public:
	void LoadFile(const char* filename, const std::wstring& textureFilename = L"");
	void LoadMaterial(std::ifstream& file, bool bCopy);

	void CreateTextures();
	void CreateMaterials();

private:
	std::vector<JHDMeshInfo>						m_Meshes;
	std::vector<std::vector<std::shared_ptr<FbxBoneInfo>>>		m_Bones;
	std::vector<std::shared_ptr<FbxAnimClipInfo>>	m_AnimClips;
	std::wstring m_ResourceDirectory;
};

