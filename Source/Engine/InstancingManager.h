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
	// 동일한 쉐이더, 메쉬, 머테리얼을 사용하면 같은 데이터로
	void AddParam(UINT64 instanceId, InstancingParams& data);

private:
	std::unordered_map<UINT64/*instanceId*/, std::shared_ptr<CInstancingBuffer>> m_Buffers;
};

