#pragma once
#include "InstancingBuffer.h"

class CGameObject;

class CInstancingManager
	: public CSingleton<CInstancingManager>
{
	DECLARE_SINGLETON(CInstancingManager)

public:
	void Render(std::vector<CGameObject*>& gameObjects);

	void ClearBuffer();
	void Clear() { m_Buffers.clear(); }

private:
	// ������ ���̴�, �޽�, ���׸����� ����ϸ� ���� �����ͷ�
	void AddParam(UINT64 instanceId, InstancingParams& data);

private:
	std::unordered_map<UINT64/*instanceId*/, std::shared_ptr<CInstancingBuffer>> m_Buffers;
};

