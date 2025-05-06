#include "pch.h"
#include "ToolTip.h"
#include "TextWidget.h"
#include "Item.h"
#include "SkillData.h"

void CToolTip::Hide()
{
	if (m_Name) m_Name->SetEnable(false);
	if (m_Description) m_Description->SetEnable(false);
	if (m_Price) m_Price->SetEnable(false);

}
