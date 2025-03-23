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

	const AnimClipInfo& animClip = m_AnimClipsMap[m_PlayAnimName];
	if (m_UpdateTime >= animClip.duration)
		m_UpdateTime = 0.f;

	const INT32 ratio = static_cast<INT32>(animClip.frameCount / animClip.duration);
	m_Frame = static_cast<INT32>(m_UpdateTime * ratio);
	m_Frame = min(m_Frame, animClip.frameCount - 1);
	m_NextFrame = min(m_Frame + 1, animClip.frameCount - 1);
	m_FrameRatio = static_cast<float>(m_Frame - m_Frame);
}

void CAnimator::SetBones(const std::vector<BoneInfo>* bones)
{
	m_Bones = bones;
}

void CAnimator::SetAnimClip(const std::vector<AnimClipInfo>* animClips)
{
	std::cout << "My name is " << ws2s(GetOwner()->GetMeshRenderer()->GetMesh()->GetName()) << ", I have " << animClips->size() << "Animation" << '\n';
	for (int i = 0; i < animClips->size(); ++i)
	{
		std::wstring name = animClips->at(i).animName;
		RemoveNamespace(name);
		m_AnimClipsMap[name] = animClips->at(i);
		m_Ws2Idx[name] = i;
		
		std::cout << "Animation Name" << i << ": " << ws2s(name) << '\n';
	}
	std::cout << '\n';
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

void CAnimator::Play(std::wstring animName)
{
	m_ClipIndex = m_Ws2Idx[animName];
	assert(m_ClipIndex < m_AnimClipsMap.size());
	m_PlayAnimName = animName;
	m_Duration = m_AnimClipsMap[m_PlayAnimName].duration;
	m_UpdateTime = 0.f;
}

void CAnimator::RemoveNamespace(std::wstring& name)
{
	size_t pos = name.find(L"|");
	if (pos != std::string::npos) {
		name = name.substr(pos + 1);
	}
}
