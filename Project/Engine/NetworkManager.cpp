#include "pch.h"
#include "NetworkManager.h"
#include <ServerCore/Service.h>
#include <ServerCore/ThreadManager.h>
#include <ServerCore/Session.h>
#include "ClientSession.h"
#include "ServerSession.h"
#include "ClientPacketHandler.h"
#include "ServerPacketHandler.h"

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
	if (CEngine::GetInst()->GetNetworkType() == ENetwork_Type::OnlineHost)
	{
		return InitHost();
	}
	else if (CEngine::GetInst()->GetNetworkType() == ENetwork_Type::OnlineClient)
	{
		return InitClient();
	}

	std::cout << "네트워크 초기화 실패" << std::endl;

	return false;
}

void CNetworkManager::ExecuteJobs()
{
	std::vector<std::shared_ptr<CJob>> vecJob;
	m_JobQueue.PopAll(vecJob);

	for (auto& job : vecJob)
		job->Execute();
}

bool CNetworkManager::InitHost()
{
	ServerPacketHandler::Init();

	CServerServiceRef service = std::make_shared<CServerService>(
		CNetAddress(L"127.0.0.1", 7777),
		std::make_shared<CIocpCore>(),
		[=]() { return std::make_shared<CClientSession>(); },
		1);

	if (!service->Start())
		return false;

	for (int i = 0; i < 4; i++)
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

bool CNetworkManager::InitClient()
{
	ClientPacketHandler::Init();

	s_GameSession = std::make_shared<CServerSession>();

	CClientServiceRef service = std::make_shared<CClientService>(
		CNetAddress(L"127.0.0.1", 7777),
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
