#include "pch.h"
#include "Mesh.h"
#include "Device.h"
#include "JHDLoader.h"
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

CMesh* CMesh::CreateFromJHD(const JHDMeshInfo* meshInfo, CJHDLoader& loader, int idx)
{
	CMesh* mesh = new CMesh;
	mesh->CreateVertexBuffer(meshInfo->vertices);
	//mesh->SetMeshSize(meshInfo->maxPos);
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

	if (meshInfo->hasAnimation)
		mesh->CreateBonesAndAnimations(loader, idx);


	return mesh;
}

int CMesh::CreateVertexBuffer(const std::vector<Vertex>& vecVertex)
{
	m_VertexCount = static_cast<UINT>(vecVertex.size());
	UINT bufferSize = m_VertexCount * sizeof(Vertex);

	D3D12_HEAP_PROPERTIES defaultHeapProps = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
	D3D12_RESOURCE_DESC bufferDesc = CD3DX12_RESOURCE_DESC::Buffer(bufferSize);

	if (FAILED(DEVICE->CreateCommittedResource(
		&defaultHeapProps,
		D3D12_HEAP_FLAG_NONE,
		&bufferDesc,
		D3D12_RESOURCE_STATE_COMMON, 
		nullptr,
		IID_PPV_ARGS(&m_VertexBuffer))))
	{
		return E_FAIL;
	}

	D3D12_HEAP_PROPERTIES uploadHeapProps = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);

	if (FAILED(DEVICE->CreateCommittedResource(
		&uploadHeapProps,
		D3D12_HEAP_FLAG_NONE,
		&bufferDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ, 
		nullptr,
		IID_PPV_ARGS(&m_VertexUploadBuffer))))
	{
		return E_FAIL;
	}

	void* vertexDataBuffer = nullptr;
	CD3DX12_RANGE readRange(0, 0); 
	m_VertexUploadBuffer->Map(0, &readRange, &vertexDataBuffer);
	::memcpy(vertexDataBuffer, vecVertex.data(), bufferSize);
	m_VertexUploadBuffer->Unmap(0, nullptr);

	auto commandList = CDevice::GetInst()->GetCmdQueue()->GetGraphicsCmdList(); 

	commandList.Get()->CopyResource(m_VertexBuffer.Get(), m_VertexUploadBuffer.Get());

	CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
		m_VertexBuffer.Get(),
		D3D12_RESOURCE_STATE_COPY_DEST,
		D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER);

	commandList->ResourceBarrier(1, &barrier);

	m_VertexBufferView.BufferLocation = m_VertexBuffer->GetGPUVirtualAddress();
	m_VertexBufferView.StrideInBytes = sizeof(Vertex);
 	m_VertexBufferView.SizeInBytes = bufferSize;

	return S_OK;
}

int CMesh::CreateIndexBuffer(const std::vector<UINT>& buffer)
{
	UINT32 indexCount = static_cast<UINT32>(buffer.size());
	UINT32 bufferSize = indexCount * sizeof(UINT32);

	D3D12_HEAP_PROPERTIES heapProperty = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
	D3D12_RESOURCE_DESC desc = CD3DX12_RESOURCE_DESC::Buffer(bufferSize);

	ComPtr<ID3D12Resource> indexBuffer;
	DEVICE->CreateCommittedResource(
		&heapProperty,
		D3D12_HEAP_FLAG_NONE,
		&desc,
		D3D12_RESOURCE_STATE_COMMON,
		nullptr,
		IID_PPV_ARGS(&indexBuffer));

	D3D12_HEAP_PROPERTIES uploadHeapProps = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	if (FAILED(DEVICE->CreateCommittedResource(
		&uploadHeapProps,
		D3D12_HEAP_FLAG_NONE,
		&desc,
		D3D12_RESOURCE_STATE_GENERIC_READ, 
		nullptr,
		IID_PPV_ARGS(&m_IndexUploadBuffer))))
	{
		return E_FAIL;
	}

	void* indexDataBuffer = nullptr;
	CD3DX12_RANGE readRange(0, 0);
	m_IndexUploadBuffer->Map(0, &readRange, &indexDataBuffer);
	::memcpy(indexDataBuffer, &buffer[0], bufferSize);
	m_IndexUploadBuffer->Unmap(0, nullptr);

	auto commandList = CDevice::GetInst()->GetCmdQueue()->GetGraphicsCmdList(); 

	commandList.Get()->CopyResource(indexBuffer.Get(), m_IndexUploadBuffer.Get());

	CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
		indexBuffer.Get(),
		D3D12_RESOURCE_STATE_COPY_DEST,
		D3D12_RESOURCE_STATE_INDEX_BUFFER);

	commandList->ResourceBarrier(1, &barrier);

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

void CMesh::CreateBonesAndAnimations(CJHDLoader& loader, int index)
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
		info.keyFrames[index].resize(ac->keyFrames[index].size());

		const INT32 boneCount = static_cast<INT32>(ac->keyFrames[index].size());
		for (INT32 b = 0; b < boneCount; b++)
		{
			auto& vec = ac->keyFrames[index][b];

			const INT32 size = static_cast<INT32>(vec.size());
			frameCount = max(frameCount, static_cast<UINT32>(size));
			info.keyFrames[index][b].resize(size);

			for (INT32 f = 0; f < size; f++)
			{
				FbxKeyFrameInfo& kf = vec[f];
				// FBX에서 파싱한 정보들로 채워준다
				KeyFrameInfo& kfInfo = info.keyFrames[index][b][f];
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

		m_AnimClips.push_back(info);
	}
#pragma endregion

#pragma region Bones
	std::vector<std::vector<std::shared_ptr<FbxBoneInfo>>>& bones = loader.GetBones();
	for (std::shared_ptr<FbxBoneInfo>& bone : bones[index])
	{
		BoneInfo boneInfo = {};
		boneInfo.parentIdx = bone->parentIndex;
		boneInfo.matOffset = GetMatrix(bone->matOffset);
		boneInfo.boneName = bone->boneName;
		m_Bones.push_back(boneInfo);
	}
#pragma endregion

#pragma region SkinData
	if (IsAnimMesh())
	{
		// BoneOffet 행렬
		const INT32 boneCount = static_cast<INT32>(m_Bones.size());
		std::vector<Matrix> offsetVec(boneCount);
		for (size_t b = 0; b < boneCount; b++)
			offsetVec[b] = m_Bones[b].matOffset;

		// OffsetMatrix StructuredBuffer 세팅
		m_OffsetBuffer = std::make_shared<CStructuredBuffer>();
		m_OffsetBuffer->Init(sizeof(Matrix), static_cast<UINT32>(offsetVec.size()), offsetVec.data());

		const INT32 animCount = static_cast<INT32>(m_AnimClips.size());
		for (INT32 i = 0; i < animCount; i++)
		{
			AnimClipInfo& animClip = m_AnimClips[i];

			// 애니메이션 프레임 정보
			std::vector<AnimFrameParams> frameParams;
			frameParams.resize(m_Bones.size() * animClip.frameCount);

			for (INT32 b = 0; b < boneCount; b++)
			{
				INT32 keyFrameCount = static_cast<INT32>(animClip.keyFrames[index][b].size());
				
				for (INT32 f = 0; f < keyFrameCount; f++)
				{
					INT32 idx = static_cast<INT32>(boneCount * f + b);
					frameParams[idx] = AnimFrameParams
					{
						Vec4(animClip.keyFrames[index][b][f].scale),
						animClip.keyFrames[index][b][f].rotation,
						Vec4(animClip.keyFrames[index][b][f].translate)
					};
				}

			}

			// StructuredBuffer 세팅
			m_FrameBuffer.push_back(std::make_shared<CStructuredBuffer>());
			m_FrameBuffer.back()->Init(sizeof(AnimFrameParams), static_cast<UINT32>(frameParams.size()), frameParams.data());
		}
	}
#pragma endregion
}
