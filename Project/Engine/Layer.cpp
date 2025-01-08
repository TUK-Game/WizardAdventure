#include "pch.h"
#include "Layer.h"
#include "GameObject.h"

CLayer::CLayer()
	: m_LayerIndex(-1)
{
}

CLayer::~CLayer()
{
	for (auto& object : m_vecObjects)
		delete object;
}

void CLayer::Begin()
{
	for (auto& object : m_vecObjects)
	{
		object->Begin();
	}
}

void CLayer::Update()
{
	for (auto& object : m_vecObjects)
	{
		object->Update();
	}
}

void CLayer::FinalUpdate()
{
	for (auto& object : m_vecObjects)
	{
		object->FinalUpdate();
	}
}
