#include "pch.h"
#include "WidgetWindow.h"
#include "Widget.h"

CWidgetWindow::CWidgetWindow() :
	m_zOrder(0)
{
}

CWidgetWindow::~CWidgetWindow()
{
}

bool CWidgetWindow::Init(CPlayer* player)
{
	m_OwnerPlayer = player;
	return true;
}

void CWidgetWindow::Update()
{
	auto	iter = m_vecWidget.begin();
	auto	iterEnd = m_vecWidget.end();

	for (; iter != iterEnd;)
	{
		if (!(*iter)->GetActive())
		{
			iter = m_vecWidget.erase(iter);
			iterEnd = m_vecWidget.end();
			continue;
		}

		else if (!(*iter)->GetEnable())
		{
			++iter;
			continue;
		}

		(*iter)->Update();

		++iter;
	}
}

void CWidgetWindow::FinalUpdate()
{
	auto	iter = m_vecWidget.begin();
	auto	iterEnd = m_vecWidget.end();

	for (; iter != iterEnd;)
	{
		if (!(*iter)->GetActive())
		{
			iter = m_vecWidget.erase(iter);
			iterEnd = m_vecWidget.end();
			continue;
		}

		else if (!(*iter)->GetEnable())
		{
			++iter;
			continue;
		}

		(*iter)->FinalUpdate();

		++iter;
	}
}

void CWidgetWindow::Render()
{
	auto	iter = m_vecWidget.begin();
	auto	iterEnd = m_vecWidget.end();

	for (; iter != iterEnd;)
	{
		if (!(*iter)->GetActive())
		{
			iter = m_vecWidget.erase(iter);
			iterEnd = m_vecWidget.end();
			continue;
		}

		else if (!(*iter)->GetEnable())
		{
			++iter;
			continue;
		}

		(*iter)->Render();

		++iter;
	}
}
