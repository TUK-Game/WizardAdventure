#include "pch.h"
#include "NetworkManager.h"
#include <ServerCore/Service.h>
#include <ServerCore/ThreadManager.h>
#include <ServerCore/Session.h>
#include "ServerSession.h"
#include "ClientPacketHandler.h"

#include "Engine.h"

std::shared_ptr<class CServerSession>	CNetworkManager::s_GameSession;

CNetworkManager::CNetworkManager()
{
}

CNetworkManager::~CNetworkManager()
{
}

bool CNetworkManager::Init()
{
	if (CEngine::GetInst()->GetNetworkType() == ENetwork_Type::Offline)
		return false;

	ClientPacketHandler::Init();

	s_GameSession = std::make_shared<CServerSession>();

	std::wstring serverIpAddress = L"127.0.0.1";
	CClientServiceRef service = std::make_shared<CClientService>(
		CNetAddress(serverIpAddress.c_str(), 7777),
		std::make_shared<CIocpCore>(),
		[=]() { return s_GameSession; },
		1);

	if (!service->Start())
		return false;

	for (int i = 0; i < 2; i++)
	{
		g_ThreadManager->Launch([=]()
			{
				while (true)
				{
					service->GetIocpCore()->Dispatch();
				}
			});
	}

	return true;
}

void CNetworkManager::ExecuteJobs()
{
	std::vector<std::shared_ptr<CJob>> vecJob;
	m_JobQueue.PopAll(vecJob);

	for (auto& job : vecJob)
		job->Execute();
}
