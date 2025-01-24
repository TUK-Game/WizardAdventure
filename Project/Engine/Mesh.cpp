#include "pch.h"
#include "Mesh.h"
#include "Device.h"
#include "FBXLoader.h"
#include "InstancingBuffer.h"
#include "StructuredBuffer.h"

CMesh::CMesh()
	: CAsset(EAsset_Type::Mesh)
	, m_VertexBufferView{}
	, m_VertexCount(0)
{
}

CMesh::~CMesh()
{
}

int CMesh::Init(const std::vector<Vertex>& vecVertex, const std::vector<UINT>& vecIndex)
{
	if (FAILED(CreateVertexBuffer(vecVertex)))
		return E_FAIL;

	if (FAILED(CreateIndexBuffer(vecIndex)))
		return E_FAIL;

	return S_OK;
}

void CMesh::Render(UINT32 instanceCount, UINT32 idx)
{
	GRAPHICS_CMD_LIST->IASetVertexBuffers(0, 1, &m_VertexBufferView); // Slot: (0~15)
	GRAPHICS_CMD_LIST->IASetIndexBuffer(&m_VecIndexInfo[idx].bufferView);

	CDevice::GetInst()->GetGraphicsDescHeap()->CommitTable();

	GRAPHICS_CMD_LIST->DrawIndexedInstanced(m_VecIndexInfo[idx].count, instanceCount, 0, 0, 0);
}

void CMesh::Render(std::shared_ptr<CInstancingBuffer>& buffer, UINT32 idx)
{
	D3D12_VERTEX_BUFFER_VIEW bufferViews[] = { m_VertexBufferView, buffer->GetBufferView() };
	GRAPHICS_CMD_LIST->IASetVertexBuffers(0, 2, bufferViews);
	GRAPHICS_CMD_LIST->IASetIndexBuffer(&m_VecIndexInfo[idx].bufferView);

	CDevice::GetInst()->GetGraphicsDescHeap()->CommitTable();

	GRAPHICS_CMD_LIST->DrawIndexedInstanced(m_VecIndexInfo[idx].count, buffer->GetCount(), 0, 0, 0);
}

CMesh* CMesh::CreateFromFBX(const FbxMeshInfo* meshInfo, FBXLoader& loader)
{
	CMesh* mesh = new CMesh;
	mesh->CreateVertexBuffer(meshInfo->vertices);
	mesh->SetMeshSize(meshInfo->maxPos);
	mesh->SetName(meshInfo->name);

	for (const std::vector<UINT32>& buffer : meshInfo->indices)
	{
		if (buffer.empty())
		{
			// FBX 파일이 이상하다. IndexBuffer가 없으면 에러 나니까 임시 처리
			std::vector<UINT32> defaultBuffer{ 0 };
			mesh->CreateIndexBuffer(defaultBuffer);
		}
		else
		{
			mesh->CreateIndexBuffer(buffer);
		}
	}

	if(meshInfo->hasAnimation)
		mesh->CreateBonesAndAnimations(loader);


	return mesh;
}

int CMesh::CreateVertexBuffer(const std::vector<Vertex>& vecVertex)
{
	m_VertexCount = static_cast<UINT>(vecVertex.size());
	UINT bufferSize = m_VertexCount * sizeof(Vertex);

	D3D12_HEAP_PROPERTIES heapProperty = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	D3D12_RESOURCE_DESC desc = CD3DX12_RESOURCE_DESC::Buffer(bufferSize);

	if (FAILED(DEVICE->CreateCommittedResource(
		&heapProperty,
		D3D12_HEAP_FLAG_NONE,
		&desc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&m_VertexBuffer))))
	{
		return E_FAIL;
	}

	// Copy the triangle data to the vertex buffer.
	void* vertexDataBuffer = nullptr;
	CD3DX12_RANGE readRange(0, 0); // We do not intend to read from this resource on the CPU.
	m_VertexBuffer->Map(0, &readRange, &vertexDataBuffer);
	::memcpy(vertexDataBuffer, &vecVertex[0], bufferSize);
	m_VertexBuffer->Unmap(0, nullptr);

	// Initialize the vertex buffer view.
	m_VertexBufferView.BufferLocation = m_VertexBuffer->GetGPUVirtualAddress();
	m_VertexBufferView.StrideInBytes = sizeof(Vertex); // 정점 1개 크기
	m_VertexBufferView.SizeInBytes = bufferSize; // 버퍼의 크기	

	return S_OK;
}

int CMesh::CreateIndexBuffer(const std::vector<UINT>& buffer)
{
	UINT32 indexCount = static_cast<UINT32>(buffer.size());
	UINT32 bufferSize = indexCount * sizeof(UINT32);

	D3D12_HEAP_PROPERTIES heapProperty = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	D3D12_RESOURCE_DESC desc = CD3DX12_RESOURCE_DESC::Buffer(bufferSize);

	ComPtr<ID3D12Resource> indexBuffer;
	DEVICE->CreateCommittedResource(
		&heapProperty,
		D3D12_HEAP_FLAG_NONE,
		&desc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&indexBuffer));

	void* indexDataBuffer = nullptr;
	CD3DX12_RANGE readRange(0, 0);
	indexBuffer->Map(0, &readRange, &indexDataBuffer);
	::memcpy(indexDataBuffer, &buffer[0], bufferSize);
	indexBuffer->Unmap(0, nullptr);

	D3D12_INDEX_BUFFER_VIEW	indexBufferView;
	indexBufferView.BufferLocation = indexBuffer->GetGPUVirtualAddress();
	indexBufferView.Format = DXGI_FORMAT_R32_UINT;
	indexBufferView.SizeInBytes = bufferSize;

	IndexBufferInfo info =
	{
		indexBuffer,
		indexBufferView,
		DXGI_FORMAT_R32_UINT,
		indexCount
	};

	m_VecIndexInfo.push_back(info);

	return S_OK;
}

void CMesh::CreateBonesAndAnimations(FBXLoader& loader)
{
#pragma region AnimClip
	UINT32 frameCount = 0;
	std::vector<std::shared_ptr<FbxAnimClipInfo>>& animClips = loader.GetAnimClip();
	for (std::shared_ptr<FbxAnimClipInfo>& ac : animClips)
	{
		AnimClipInfo info = {};

		info.animName = ac->name;
		info.duration = ac->endTime.GetSecondDouble() - ac->startTime.GetSecondDouble();

		INT32 startFrame = static_cast<INT32>(ac->startTime.GetFrameCount(ac->mode));
		INT32 endFrame = static_cast<INT32>(ac->endTime.GetFrameCount(ac->mode));
		info.frameCount = endFrame - startFrame;

		info.keyFrames.resize(ac->keyFrames.size());

		const INT32 boneCount = static_cast<INT32>(ac->keyFrames.size());
		for (INT32 b = 0; b < boneCount; b++)
		{
			auto& vec = ac->keyFrames[b];

			const INT32 size = static_cast<INT32>(vec.size());
			frameCount = max(frameCount, static_cast<UINT32>(size));
			info.keyFrames[b].resize(size);

			for (INT32 f = 0; f < size; f++)
			{
				FbxKeyFrameInfo& kf = vec[f];
				// FBX에서 파싱한 정보들로 채워준다
				KeyFrameInfo& kfInfo = info.keyFrames[b][f];
				kfInfo.time = kf.time;
				kfInfo.frame = static_cast<INT32>(size);
				kfInfo.scale.x = static_cast<float>(kf.matTransform.GetS().mData[0]);
				kfInfo.scale.y = static_cast<float>(kf.matTransform.GetS().mData[1]);
				kfInfo.scale.z = static_cast<float>(kf.matTransform.GetS().mData[2]);
				kfInfo.rotation.x = static_cast<float>(kf.matTransform.GetQ().mData[0]);
				kfInfo.rotation.y = static_cast<float>(kf.matTransform.GetQ().mData[1]);
				kfInfo.rotation.z = static_cast<float>(kf.matTransform.GetQ().mData[2]);
				kfInfo.rotation.w = static_cast<float>(kf.matTransform.GetQ().mData[3]);
				kfInfo.translate.x = static_cast<float>(kf.matTransform.GetT().mData[0]);
				kfInfo.translate.y = static_cast<float>(kf.matTransform.GetT().mData[1]);
				kfInfo.translate.z = static_cast<float>(kf.matTransform.GetT().mData[2]);
			}
		}

		_animClips.push_back(info);
	}
#pragma endregion

#pragma region Bones
	std::vector<std::shared_ptr<FbxBoneInfo>>& bones = loader.GetBones();
	for (std::shared_ptr<FbxBoneInfo>& bone : bones)
	{
		BoneInfo boneInfo = {};
		boneInfo.parentIdx = bone->parentIndex;
		boneInfo.matOffset = GetMatrix(bone->matOffset);
		boneInfo.boneName = bone->boneName;
		_bones.push_back(boneInfo);
	}
#pragma endregion

#pragma region SkinData
	if (IsAnimMesh())
	{
		// BoneOffet 행렬
		const INT32 boneCount = static_cast<INT32>(_bones.size());
		std::vector<Matrix> offsetVec(boneCount);
		for (size_t b = 0; b < boneCount; b++)
			offsetVec[b] = _bones[b].matOffset;

		// OffsetMatrix StructuredBuffer 세팅
		_offsetBuffer = std::make_shared<CStructuredBuffer>();
		_offsetBuffer->Init(sizeof(Matrix), static_cast<UINT32>(offsetVec.size()), offsetVec.data());

		const INT32 animCount = static_cast<INT32>(_animClips.size());
		for (INT32 i = 0; i < animCount; i++)
		{
			AnimClipInfo& animClip = _animClips[i];

			// 애니메이션 프레임 정보
			std::vector<AnimFrameParams> frameParams;
			frameParams.resize(_bones.size() * animClip.frameCount);

			for (INT32 b = 0; b < boneCount; b++)
			{
				const INT32 keyFrameCount = static_cast<INT32>(animClip.keyFrames[b].size());
				for (INT32 f = 0; f < keyFrameCount; f++)
				{
					INT32 idx = static_cast<INT32>(boneCount * f + b);

					frameParams[idx] = AnimFrameParams
					{
						Vec4(animClip.keyFrames[b][f].scale),
						animClip.keyFrames[b][f].rotation, // Quaternion
						Vec4(animClip.keyFrames[b][f].translate)
					};
				}
			}

			// StructuredBuffer 세팅
			_frameBuffer.push_back(std::make_shared<CStructuredBuffer>());
			_frameBuffer.back()->Init(sizeof(AnimFrameParams), static_cast<UINT32>(frameParams.size()), frameParams.data());
		}
	}
#pragma endregion
}

Matrix CMesh::GetMatrix(FbxAMatrix& matrix)
{
	Matrix mat;

	for (INT32 y = 0; y < 4; ++y)
		for (INT32 x = 0; x < 4; ++x)
			mat.m[y][x] = static_cast<float>(matrix.Get(y, x));

	return mat;
}
