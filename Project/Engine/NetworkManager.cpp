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

	std::cout << "Please input server IP address" << std::endl;
	std::cout << "(Input 0 = 127.0.0.1)" << std::endl;
	std::cout << std::endl;
	std::cout << "input : ";

	std::wstring serverIpAddress;
	std::wcin >> serverIpAddress;

	if (serverIpAddress.size() == 1 && serverIpAddress[0] == '0')
		serverIpAddress = L"127.0.0.1";

	CClientServiceRef service = std::make_shared<CClientService>(
		CNetAddress(serverIpAddress.c_str(), 6767),
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
