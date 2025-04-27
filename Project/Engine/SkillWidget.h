#pragma once
#include "ImageWidget.h"
class CSkillWidget :
	public CImageWidget
{
public:
	virtual void SetTexture(const std::wstring& name);
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

};

