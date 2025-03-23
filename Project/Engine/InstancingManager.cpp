#include "pch.h"
#include "InstancingManager.h"
#include "InstancingBuffer.h"
#include "GameObject.h"
#include "MeshRenderer.h"
#include "Transform.h"
#include "Camera.h"
#include "Logger.h"
#include "AssetManager.h"

CInstancingManager::CInstancingManager()
{
}

CInstancingManager::~CInstancingManager()
{

}

void CInstancingManager::Render(std::vector<CGameObject*>& gameObjects, const std::wstring& materialName)	
{
	if(materialName == L"DeferredMap")
	{
		CAssetManager::GetInst()->FindAsset<CMaterial>(materialName)->GraphicsBinding();
	}


	// ���� �ν��Ͻ� ���̵� ���� ������Ʈ���� ������ ĳ��
	std::unordered_map<UINT64, std::vector<CGameObject*>> cache;

	for (CGameObject* gameObject : gameObjects)
	{
		const UINT64 instanceId = gameObject->GetMeshRenderer()->GetInstanceID();
		cache[instanceId].push_back(gameObject);
	}

	for (auto& pair : cache)
	{
		const std::vector<CGameObject*>& vec = pair.second;

		// ��ü�� �ϳ��� ����ó�� ������
		// 2�� �̻��̸� �ν��Ͻ�

		// �ӽ��� ó�� 
		if (vec.size() == 1)
		{
			vec[0]->GetMeshRenderer()->GetMaterial()->SetInt(0, 0);
			if (materialName == L"DeferredMap")
			{
				vec[0]->GetMeshRenderer()->RenderMap();
			}
			else
			{
				vec[0]->GetMeshRenderer()->Render();
			}
		}
		else
		{
			const UINT64 instanceId = pair.first;
			if(vec[0]->GetInstancing())
				vec[0]->GetMeshRenderer()->GetMaterial()->SetInt(0, 1);
			else
				vec[0]->GetMeshRenderer()->GetMaterial()->SetInt(0, 0);

			if (!vec[0]->GetMeshRenderer()->GetMaterial()->GetInt(0))
			{
				for (const auto& obj : vec)
					obj->GetMeshRenderer()->Render();
				continue;
			}

			for (CGameObject* gameObject : vec)
			{
				InstancingParams params;
				params.matWorld = gameObject->GetTransform()->GetWorldMatrix();
				params.matWV = params.matWorld * CCamera::s_matView;
				params.matWVP = params.matWorld * CCamera::s_matView * CCamera::s_matProjection;

				// �ν��Ͻ� ���̵� �ش��ϴ� ���� ã�� ���� �� �߰�
				AddParam(instanceId, params);
			}	

			std::shared_ptr<CInstancingBuffer>& buffer = m_Buffers[instanceId];

			if(materialName == L"DeferredMap")
				vec[0]->GetMeshRenderer()->RenderMap(buffer);	
			else
				vec[0]->GetMeshRenderer()->Render(buffer);	
		}
	}
}

void CInstancingManager::ClearBuffer()
{
	for (auto& pair : m_Buffers)
	{
		std::shared_ptr<CInstancingBuffer>& buffer = pair.second;
		buffer->Clear();
	}
}

void CInstancingManager::AddParam(UINT64 instanceId, InstancingParams& data)
{
	if (m_Buffers.find(instanceId) == m_Buffers.end())
		m_Buffers[instanceId] = std::make_shared<CInstancingBuffer>();

	m_Buffers[instanceId]->AddData(data);
}