#pragma once

#include "FBX/fbxsdk.h"
#include "Global.h"
#include <array>

struct FbxMaterialInfo
{
	float			diffuse[4];
	float			ambient[4];
	float			specular[4];
	std::wstring			name;
	std::wstring			diffuseTexName;
	std::wstring			normalTexName;
	std::wstring			specularTexName;
};

class Vertex
{
public:
	float	position[3]{};
	float	uv[2]{};
	float	normal[3]{};
	float	tangent[3]{};
	float	biNormal[3]{};
	float	weights[4]{};
	float	indices[4]{};
	INT32   controlPoint;
	Vertex() = default;
	Vertex(std::vector<float>& _position, std::vector<float>& _normal, std::vector<float>& _tangent, std::vector<float>& _biNormal, std::vector<float>& _uv)
	{
		position[0] = _position[0];
		position[1] = _position[1];
		position[2] = _position[2];
		normal[0] = _normal[0];
		normal[1] = _normal[1];
		normal[2] = _normal[2];
		biNormal[0] = _biNormal[0];
		biNormal[1] = _biNormal[1];
		biNormal[2] = _biNormal[2];
		tangent[0] = _tangent[0];
		tangent[1] = _tangent[1];
		tangent[2] = _tangent[2];
		uv[0] = _uv[0];
		uv[1] = _uv[1];
	}
	Vertex(const Vertex& other) = default;

	Vertex(Vertex&& other) noexcept = default;

	Vertex& operator=(const Vertex& other) = default;

	Vertex& operator=(Vertex&& other) noexcept = default;

	bool operator==(const Vertex& other) const noexcept
	{
		return std::memcmp(position, other.position, sizeof(position)) == 0 &&
			std::memcmp(uv, other.uv, sizeof(uv)) == 0 &&
			std::memcmp(normal, other.normal, sizeof(normal)) == 0 &&
			std::memcmp(tangent, other.tangent, sizeof(tangent)) == 0;
	}
};

namespace std
{
	template <>
	struct hash<Vertex>
	{
		std::size_t operator()(const Vertex& v) const noexcept
		{
			std::size_t h1 = std::hash<float>{}(v.position[0]) ^ (std::hash<float>{}(v.position[1]) << 1) ^ (std::hash<float>{}(v.position[2]) << 2);
			std::size_t h2 = std::hash<float>{}(v.normal[0]) ^ (std::hash<float>{}(v.normal[1]) << 1) ^ (std::hash<float>{}(v.normal[2]) << 2);
			std::size_t h3 = std::hash<float>{}(v.tangent[0]) ^ (std::hash<float>{}(v.tangent[1]) << 1) ^ (std::hash<float>{}(v.tangent[2]) << 2);
			std::size_t h4 = std::hash<float>{}(v.uv[0]) ^ (std::hash<float>{}(v.uv[1]) << 1);

			return h1 ^ (h2 << 3) ^ (h3 << 6) ^ (h4 << 9);
		}
	};
}

struct Mesh
{
	std::vector<Vertex> vertices;
	std::vector<int> indices;
};

struct BoneWeight
{
	using Pair = std::pair<INT32, double>;
	std::vector<Pair> boneWeights;

	void AddWeights(UINT32 index, double weight)
	{
		if (weight <= 0.f)
			return;

		auto findIt = std::find_if(boneWeights.begin(), boneWeights.end(),
			[=](const Pair& p) { return p.second < weight; });

		if (findIt != boneWeights.end())
			boneWeights.insert(findIt, Pair(index, weight));
		else
			boneWeights.push_back(Pair(index, weight));

		// 가중치는 최대 4개
		if (boneWeights.size() > 4)
			boneWeights.pop_back();
	}

	void Normalize()
	{
		double sum = 0.f;
		std::for_each(boneWeights.begin(), boneWeights.end(), [&](Pair& p) { sum += p.second; });
		std::for_each(boneWeights.begin(), boneWeights.end(), [=](Pair& p) { p.second = p.second / sum; });
	}
};


struct FbxMeshInfo
{
	std::wstring							name;
	std::vector<Vertex>						vertices;
	std::vector<std::vector<UINT32>>		indices;
	std::vector<FbxMaterialInfo>			materials;
	std::vector<BoneWeight>					boneWeights; // 뼈 가중치
	bool									hasAnimation;
	float minPos[3];
	float maxPos[3];
	float centerPos[4];
	FbxAMatrix matrix;
	float translate[4];
	float rotation[4];
	float scale[4];
};

struct FbxKeyFrameInfo
{
	FbxAMatrix  matTransform;
	double		time;
};

struct FbxBoneInfo
{
	std::wstring			boneName;
	INT32					parentIndex;
	FbxAMatrix				matOffset;
};

struct FbxAnimClipInfo
{
	std::wstring			name;
	FbxTime					startTime;
	FbxTime					endTime;
	FbxTime::EMode			mode;
	std::array<std::vector<std::vector<FbxKeyFrameInfo>>, 100>	keyFrames;
};

class CFBXConverter
{
public:
	~CFBXConverter();

public:
	void LoadFBX(const char* filename);

private:
	void LoadMesh(FbxMesh* mesh);

	void Parsing(FbxNode* node);
	void LoadMaterial(FbxSurfaceMaterial* surfaceMaterial);

	void LoadBones(FbxNode* node) { LoadBones(node, 0, -1); }
	void LoadBones(FbxNode* node, INT32 idx, INT32 parentIdx);
	void LoadAnimationInfo();
	void LoadAnimationData(FbxMesh* mesh, FbxMeshInfo* meshInfo);
	void LoadBoneWeight(FbxCluster* cluster, INT32 boneIdx, FbxMeshInfo* meshInfo);
	void LoadOffsetMatrix(FbxCluster* cluster, const FbxAMatrix& matNodeTransform, INT32 boneIdx, FbxMeshInfo* meshInfo);
	void LoadKeyframe(INT32 animIndex, FbxNode* node, FbxCluster* cluster, const FbxAMatrix& matNodeTransform, INT32 boneIdx, FbxMeshInfo* container);

	std::vector<float> GetTangent(FbxMesh* mesh, uint32_t controlPointIndex, uint32_t vertexCounter);
	std::vector<float> GetNormal(FbxMesh* mesh, uint32_t controlPointIndex, uint32_t vertexCounter);
	std::vector<float> GetBiNormal(FbxMesh* mesh, uint32_t controlPointIndex, uint32_t vertexCounter);
	std::vector<float> GetUV(FbxMesh* mesh, uint32_t controlPointIndex, uint32_t vertexCounter);
	float* GetMaterialData(FbxSurfaceMaterial* surface, const char* materialName, const char* factorName);
	std::wstring GetTextureRelativeName(FbxSurfaceMaterial* surface, const char* materialProperty);

	INT32 FindBoneIndex(std::string name);
	FbxAMatrix GetTransform(FbxNode* node);
	void FillBoneWeight(FbxMesh* mesh, FbxMeshInfo* meshInfo);

	void SaveBinary(const char* filename, const std::vector<FbxMeshInfo>& mesh);

	void RemoveNumber(std::string& name);

	void WriteString(std::string& str, std::ofstream& file);
	void WriteString(const char* str, std::ofstream& file);
	void WriteEndl(std::string& str, std::ofstream& file);

	void GetControlPoints(FbxMesh* mesh, std::vector < std::vector<float> >& pos, FbxMeshInfo& info);
	void InsertVertex(std::vector<float>& position, std::vector<float>& normal, std::vector<float>& tangent, std::vector<float>& biNormal, std::vector<float>& uv, FbxMeshInfo& info, std::unordered_map<Vertex, uint16_t>& indexMapping);
private:
	std::vector<FbxMeshInfo> m_Meshes;
	FbxManager* m_Manager = nullptr;
	FbxScene* m_Scene = nullptr;
	FbxImporter* m_Importer = nullptr;

	std::vector<std::shared_ptr<FbxBoneInfo>>		m_Bones;
	std::vector<std::vector<std::shared_ptr<FbxBoneInfo>>>		m_Bone;
	std::vector<std::shared_ptr<FbxAnimClipInfo>>	m_AnimClips;
	FbxArray<FbxString*>							m_AnimNames;
	int count = 0;
};

