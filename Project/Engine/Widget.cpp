#include "pch.h"
#include "Widget.h"
#include "WidgetWindow.h"
#include "Component.h"
#include "MeshRenderer.h"
#include "AssetManager.h"

CWidget::CWidget() :
	m_owner(nullptr),
	m_mouseHovered(false)
{
}

CWidget::~CWidget()
{
}

bool CWidget::Init()
{
	return true;
}

void CWidget::Update()
{
	CGameObject::Update();
}

void CWidget::FinalUpdate()
{
	CGameObject::FinalUpdate();
}

void CWidget::Render()
{
	CGameObject::Render();
}

