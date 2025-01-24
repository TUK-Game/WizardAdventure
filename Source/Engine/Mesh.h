#pragma once

#include "Asset.h"

struct IndexBufferInfo
{
	ComPtr<ID3D12Resource>		buffer;
	D3D12_INDEX_BUFFER_VIEW		bufferView;
	DXGI_FORMAT					format;
	UINT32						count;
};

struct KeyFrameInfo
{
	double	time;
	INT32	frame;
	Vec3	scale;
	Vec4	rotation;
	Vec3	translate;
};

struct BoneInfo
{
	std::wstring					boneName;
	INT32					parentIdx;
	Matrix					matOffset;
};

struct AnimClipInfo
{
	std::wstring			animName;
	INT32			frameCount;
	double			duration;
	std::vector<std::vector<KeyFrameInfo>>	keyFrames;
};

class CStructuredBuffer;

class CMesh :
	public CAsset
{
public:
    CMesh();
	CMesh(const CMesh& other) = delete;
    ~CMesh();

public:
	int Init(const std::vector<Vertex>& vecVertex, const std::vector<UINT>& vecIndex);
	void Render(UINT32 instanceCount = 1, UINT32 idx = 0);
	void Render(std::shared_ptr<class CInstancingBuffer>& buffer, UINT32 idx = 0);

	static CMesh* CreateFromFBX(const struct FbxMeshInfo* meshInfo, class FBXLoader& loader);
private:
	int CreateVertexBuffer(const std::vector<Vertex>& vecVertex);
	int CreateIndexBuffer(const std::vector<UINT>& buffer);
	void CreateBonesAndAnimations(class FBXLoader& loader);
	Matrix GetMatrix(FbxAMatrix& matrix);

public:
	UINT32 GetSubsetCount() { return static_cast<UINT32>(m_VecIndexInfo.size()); }
	void SetMeshSize(Vec3 size) { m_MeshSize = size; }
	Vec3 GetMeshSize() { return m_MeshSize; }

	const std::vector<BoneInfo>* GetBones() { return &_bones; }
	UINT32						GetBoneCount() { return static_cast<UINT32>(_bones.size()); }
	const std::vector<AnimClipInfo>* GetAnimClip() { return &_animClips; }

	bool							IsAnimMesh() { return !_animClips.empty(); }
	std::shared_ptr<CStructuredBuffer>	GetBoneFrameDataBuffer(INT32 index = 0) { return _frameBuffer[index]; } // 전체 본 프레임 정보
	std::shared_ptr<CStructuredBuffer>	GetBoneOffsetBuffer() { return  _offsetBuffer; }

public:
	virtual CMesh* Clone() override { return nullptr; }

private:
	ComPtr<ID3D12Resource>		m_VertexBuffer;
	D3D12_VERTEX_BUFFER_VIEW	m_VertexBufferView;
	UINT						m_VertexCount;

	std::vector<IndexBufferInfo>		m_VecIndexInfo;

	Vec3 m_MeshSize;

	// Animation
	std::vector<AnimClipInfo>			_animClips;
	std::vector<BoneInfo>				_bones;

	std::shared_ptr<CStructuredBuffer>	_offsetBuffer; // 각 뼈의 offset 행렬
	std::vector<std::shared_ptr<CStructuredBuffer>> _frameBuffer; // 전체 본 프레임 정보
};

