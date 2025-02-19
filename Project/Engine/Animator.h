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

	INT32 GetAnimCount() { return static_cast<UINT32>(m_AnimClips->size()); }
	INT32 GetCurrentClipIndex() { return _clipIndex; }
	void Play(UINT32 idx);

public:
	virtual void FinalUpdate() override;

public:
	virtual CAnimator* Clone() override { return new CAnimator(*this); }

private:
	const std::vector<BoneInfo>* m_Bones;
	const std::vector<AnimClipInfo>* m_AnimClips;

	float							_updateTime = 0.f;
	INT32							_clipIndex = 0;
	INT32							_frame = 0;
	INT32							_nextFrame = 0;
	float							_frameRatio = 0;

	CMaterial*			_computeMaterial;
	std::shared_ptr<CStructuredBuffer>	_boneFinalMatrix;  // 특정 프레임의 최종 행렬
	bool							_boneFinalUpdated = false;
};
