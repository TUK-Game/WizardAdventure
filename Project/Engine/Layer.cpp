#include "pch.h"
#include "Layer.h"
#include "GameObject.h"

CLayer::CLayer()
	: m_LayerIndex(-1)
{
}

CLayer::~CLayer()
{
	for (auto& object : m_vecParentObjects)
		delete object;
}

void CLayer::AddGameObject(CGameObject* parent, bool bChildMove)
{
	m_vecParentObjects.push_back(parent);
	parent->m_LayerIndex = m_LayerIndex;

	// �ڽ��� �ִٸ�, �ڽĵ� �ش� ���̾� �Ҽ����� ����
	static std::list<CGameObject*> queue;
	queue.clear();
	queue.push_back(parent);

	while (!queue.empty())
	{
		CGameObject* pObject = queue.front();
		queue.pop_front();

		const std::vector<CGameObject*>& vecChild = pObject->GetChild();
		for (size_t i = 0; i < vecChild.size(); ++i)
		{
			queue.push_back(vecChild[i]);
		}

		// case 1. �ֻ��� �θ� ������Ʈ
		// case 2. �ڽĿ�����Ʈ���� �ڽı��� ���� �̵��ϱ�� �� ��� 
		// case 3. �ֻ����θ������Ʈ X, �ڽ��̵� X �� �Ҽ��� ���� ������Ʈ�� ���
		if (!pObject->GetParent() || bChildMove || pObject->m_LayerIndex == -1)
		{
			pObject->m_LayerIndex = m_LayerIndex;
		}
	}
}

void CLayer::Begin()
{
	for (auto& object : m_vecParentObjects)
	{
		object->Begin();
	}
}

void CLayer::Update()
{
	for (auto& object : m_vecParentObjects)
	{
		object->Update();
	}
}

void CLayer::FinalUpdate()
{
	for (auto& object : m_vecParentObjects)
	{
		object->FinalUpdate();
	}
}
