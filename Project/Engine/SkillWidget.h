#pragma once
#include "ImageWidget.h"
#include "SkillInfo.h"

class CSkillWidget :
	public CImageWidget
{
public:
	virtual void SetTexture(const std::wstring& name);
	virtual void SetMesh(const std::wstring& name);
	void SetSlot(ESkillSlot slot) { m_slot = slot; }

	void SetCoolTime(float coolTime) { m_CoolTime = coolTime; }
	void SetOriginCoolTime(float OriginCool) { m_OriginCoolTime = OriginCool; }

public:
	virtual bool Init(class CPlayer* player) override;
	virtual void Update() override;

public:
	virtual CSkillWidget* Clone() override { return new CSkillWidget(*this); }

private:
	float m_OriginCoolTime{};
	float m_CoolTime{};
	ESkillSlot m_slot;

};

