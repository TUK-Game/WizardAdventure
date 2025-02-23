#pragma once
#include "Component.h"
#include "Mesh.h"

class CMaterial;
class CStructuredBuffer;
class CMesh;

class CAnimator : public CComponent
{
public:
	CAnimator();
	virtual ~CAnimator();

public:
	void SetBones(const std::vector<BoneInfo>* bones) { m_Bones = bones; }
	void SetAnimClip(const std::vector<AnimClipInfo>* animClips);
	void PushData();

	const std::vector<AnimClipInfo>* GetAnimClips() const { return m_AnimClips; }
	INT32 GetAnimCount() { return static_cast<UINT32>(m_AnimClips->size()); }
	INT32 GetCurrentClipIndex() { return m_ClipIndex; }
	void Play(UINT32 idx);

public:
	virtual void FinalUpdate() override;

public:
	virtual CAnimator* Clone() override { return new CAnimator(*this); }

private:
	const std::vector<BoneInfo>* m_Bones;
	const std::vector<AnimClipInfo>* m_AnimClips;

	float							m_UpdateTime = 0.f;
	INT32							m_ClipIndex = 0;
	INT32							m_Frame = 0;
	INT32							m_NextFrame = 0;
	float							m_FrameRatio = 0;

	CMaterial*			m_ComputeMaterial;
	std::shared_ptr<CStructuredBuffer>	m_BoneFinalMatrix;  // 특정 프레임의 최종 행렬
	bool							m_BoneFinalUpdated = false;
};
