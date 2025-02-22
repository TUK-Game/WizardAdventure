#include "pch.h"
#include "Animator.h"
#include "Timer.h"
#include "AssetManager.h"
#include "Material.h"
#include "Engine.h"
#include "Mesh.h"
#include "MeshRenderer.h"
#include "StructuredBuffer.h"

CAnimator::CAnimator() : CComponent(EComponent_Type::Animator)
{
	_computeMaterial = CAssetManager::GetInst()->FindAsset<CMaterial>(L"ComputeAnimation");

	_boneFinalMatrix = std::make_shared<CStructuredBuffer>();
}

CAnimator::~CAnimator()
{
}

void CAnimator::FinalUpdate()
{
	_updateTime += DELTA_TIME;

	const AnimClipInfo& animClip = m_AnimClips->at(_clipIndex);
	if (_updateTime >= animClip.duration)
		_updateTime = 0.f;

	const INT32 ratio = static_cast<INT32>(animClip.frameCount / animClip.duration);
	_frame = static_cast<INT32>(_updateTime * ratio);
	_frame = min(_frame, animClip.frameCount - 1);
	_nextFrame = min(_frame + 1, animClip.frameCount - 1);
	_frameRatio = static_cast<float>(_frame - _frame);
}

void CAnimator::SetAnimClip(const std::vector<AnimClipInfo>* animClips)
{
	m_AnimClips = animClips;
}

void CAnimator::PushData()
{
	UINT32 boneCount = static_cast<UINT32>(m_Bones->size());
	if (_boneFinalMatrix->GetElementCount() < boneCount)
		_boneFinalMatrix->Init(sizeof(Matrix), boneCount);

	// Compute Shader
	CMesh* mesh = GetOwner()->GetMeshRenderer()->GetMesh();
	mesh->GetBoneFrameDataBuffer(_clipIndex)->PushComputeSRVData(SRV_REGISTER::t8);
	mesh->GetBoneOffsetBuffer()->PushComputeSRVData(SRV_REGISTER::t9);

	_boneFinalMatrix->PushComputeUAVData(UAV_REGISTER::u0);
	
	_computeMaterial->SetInt(0, boneCount);
	_computeMaterial->SetInt(1, _frame);
	_computeMaterial->SetInt(2, _nextFrame);
	_computeMaterial->SetFloat(0, _frameRatio);

	UINT32 groupCount = (boneCount / 256) + 1;
	_computeMaterial->Dispatch(groupCount, 1, 1);

	// Graphics Shader
	_boneFinalMatrix->PushGraphicsData(SRV_REGISTER::t7);
}

void CAnimator::Play(UINT32 idx)
{
	assert(idx < m_AnimClips->size());
	_clipIndex = idx;
	_updateTime = 0.f;
}