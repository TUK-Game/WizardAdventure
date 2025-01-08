#include "pch.h"
#include "LevelManager.h"
#include "Level.h"

CLevelManager::CLevelManager()
	: m_CurLevel(nullptr)
{
}

CLevelManager::~CLevelManager()
{
	delete m_CurLevel;
}

int CLevelManager::Init()
{
	m_CurLevel = new CLevel;
	m_CurLevel->Begin();

	return S_OK;
}

void CLevelManager::Progress()
{
}
