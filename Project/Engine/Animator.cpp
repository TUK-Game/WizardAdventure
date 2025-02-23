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
	m_ComputeMaterial = CAssetManager::GetInst()->FindAsset<CMaterial>(L"ComputeAnimation");

	m_BoneFinalMatrix = std::make_shared<CStructuredBuffer>();
}

CAnimator::~CAnimator()
{
}

void CAnimator::FinalUpdate()
{
	m_UpdateTime += DELTA_TIME;

	const AnimClipInfo& animClip = m_AnimClips->at(m_ClipIndex);
	if (m_UpdateTime >= animClip.duration)
		m_UpdateTime = 0.f;

	const INT32 ratio = static_cast<INT32>(animClip.frameCount / animClip.duration);
	m_Frame = static_cast<INT32>(m_UpdateTime * ratio);
	m_Frame = min(m_Frame, animClip.frameCount - 1);
	m_NextFrame = min(m_Frame + 1, animClip.frameCount - 1);
	m_FrameRatio = static_cast<float>(m_Frame - m_Frame);
}

void CAnimator::SetAnimClip(const std::vector<AnimClipInfo>* animClips)
{
	m_AnimClips = animClips;
}

void CAnimator::PushData()
{
	UINT32 boneCount = static_cast<UINT32>(m_Bones->size());
	if (m_BoneFinalMatrix->GetElementCount() < boneCount)
		m_BoneFinalMatrix->Init(sizeof(Matrix), boneCount);

	// Compute Shader
	CMesh* mesh = GetOwner()->GetMeshRenderer()->GetMesh();
	mesh->GetBoneFrameDataBuffer(m_ClipIndex)->PushComputeSRVData(SRV_REGISTER::t8);
	mesh->GetBoneOffsetBuffer()->PushComputeSRVData(SRV_REGISTER::t9);

	m_BoneFinalMatrix->PushComputeUAVData(UAV_REGISTER::u0);
	
	m_ComputeMaterial->SetInt(0, boneCount);
	m_ComputeMaterial->SetInt(1, m_Frame);
	m_ComputeMaterial->SetInt(2, m_NextFrame);
	m_ComputeMaterial->SetFloat(0, m_FrameRatio);

	UINT32 groupCount = (boneCount / 256) + 1;
	m_ComputeMaterial->Dispatch(groupCount, 1, 1);

	// Graphics Shader
	m_BoneFinalMatrix->PushGraphicsData(SRV_REGISTER::t7);
}

void CAnimator::Play(UINT32 idx)
{
	assert(idx < m_AnimClips->size());
	m_ClipIndex = idx;
	m_UpdateTime = 0.f;
}