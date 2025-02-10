#pragma once

struct FbxMaterialInfo
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
	Vec3 maxPos{};
	Vec3 minPos{};
	FbxAMatrix matrix;
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
	std::vector<std::vector<FbxKeyFrameInfo>>	keyFrames;
};

class FBXLoader
{
public:
	FBXLoader();
	~FBXLoader();

public:
	void LoadFbx(const std::wstring& path);

public:
	INT32 GetMeshCount() { return static_cast<INT32>(_meshes.size()); }
	const FbxMeshInfo& GetMesh(INT32 idx) { return _meshes[idx]; }
	std::vector<std::shared_ptr<FbxBoneInfo>>& GetBones() { return _bones; }
	std::vector<std::shared_ptr<FbxAnimClipInfo>>& GetAnimClip() { return _animClips; }
private:
	void Import(const std::wstring& path);

	void ParseNode(FbxNode* root);
	void LoadMesh(FbxMesh* mesh);
	void LoadMaterial(FbxSurfaceMaterial* surfaceMaterial);

	void		GetNormal(FbxMesh* mesh, FbxMeshInfo* container, INT32 idx, INT32 vertexCounter);
	void		GetTangent(FbxMesh* mesh, FbxMeshInfo* container, INT32 idx, INT32 vertexCounter);
	void		GetUV(FbxMesh* mesh, FbxMeshInfo* container, INT32 idx, INT32 vertexCounter);
	Vec4		GetMaterialData(FbxSurfaceMaterial* surface, const char* materialName, const char* factorName);
	std::wstring		GetTextureRelativeName(FbxSurfaceMaterial* surface, const char* materialProperty);

	void CreateTextures();
	void CreateMaterials();

	// Animation
	void LoadBones(FbxNode* node) { LoadBones(node, 0, -1); }
	void LoadBones(FbxNode* node, INT32 idx, INT32 parentIdx);
	void LoadAnimationInfo();

	void LoadAnimationData(FbxMesh* mesh, FbxMeshInfo* meshInfo);
	void LoadBoneWeight(FbxCluster* cluster, INT32 boneIdx, FbxMeshInfo* meshInfo);
	void LoadOffsetMatrix(FbxCluster* cluster, const FbxAMatrix& matNodeTransform, INT32 boneIdx, FbxMeshInfo* meshInfo);
	void LoadKeyframe(INT32 animIndex, FbxNode* node, FbxCluster* cluster, const FbxAMatrix& matNodeTransform, INT32 boneIdx, FbxMeshInfo* container);

	INT32 FindBoneIndex(std::string name);
	FbxAMatrix GetTransform(FbxNode* node);
	FbxAMatrix GetLocalTransform(FbxNode* node);

	void FillBoneWeight(FbxMesh* mesh, FbxMeshInfo* meshInfo);

private:
	FbxManager* _manager = nullptr;
	FbxScene* _scene = nullptr;
	FbxImporter* _importer = nullptr;
	std::wstring			_resourceDirectory;

	std::vector<FbxMeshInfo>					_meshes;
	std::vector<std::shared_ptr<FbxBoneInfo>>		_bones;
	std::vector<std::shared_ptr<FbxAnimClipInfo>>	_animClips;
	FbxArray<FbxString*>				_animNames;
};
