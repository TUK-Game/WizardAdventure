#pragma once
#include "ButtonWidget.h"
class CSkillButtonWidget
	: public CButtonWidget
{
	friend class CWidgetWindow;
public:
	CSkillButtonWidget();
	virtual ~CSkillButtonWidget();

public:
	void SetSkillId(uint32 id) { m_SkillId = id; }
	void SetSkill(std::shared_ptr<class CSkillData> item) { m_Skill = item; }

	uint32 GetSkillId() { return m_SkillId; }
	std::shared_ptr<class CSkillData> GetSkill() { return m_Skill; }
public:
	virtual bool Init(class CPlayer* none) override;
	virtual void Update() override;

public:
	virtual CSkillButtonWidget* Clone() override { return new CSkillButtonWidget(*this); }

private:
	uint32 m_SkillId;
	std::shared_ptr<class CSkillData> m_Skill;
};

