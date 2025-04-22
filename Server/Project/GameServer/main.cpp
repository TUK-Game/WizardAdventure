#include "pch.h"
#include <ThreadManager.h>
#include <Service.h>

#include "GameSessionManager.h"
#include "GameSession.h"
#include "Protocol.pb.h"
#include "Room.h"
#include "JsonLoader.h"
#include "CollisionManager.h"
#include "ProjectilePool.h"

enum
{
	WORKER_TICK = 64	// Job 처리 timeout
};

void DoWorkerJob(CServerServiceRef& service)
{
	while (true)
	{
		tl_EndTickCount = ::GetTickCount64() + WORKER_TICK;

		// 네트워크 입출력 처리 -> 인게임 로직까지 (패킷 핸들러에 의해)
		// Dispatch(10) : 10ms 안에 패킷이 도착하지 않았다면 Dispatch 함수 return
		service->GetIocpCore()->Dispatch(10);

		// JobQueue::DoTimer로 예약된 Job 처리
		CThreadManager::DistributeReservedJobs();

		// Job이 존재한다면 실행
		CThreadManager::DoGlobalQueueWork();
	}
}

int main()
{
	std::wstring ipAddress = CUtil::GetMyIPv4Address();
	ipAddress = L"127.0.0.1";

	ServerPacketHandler::Init();
	g_CollisionManager->Init();
	g_Room->Init();
	CJsonLoader::LoadMap(L"Level_1", g_Room);
	g_Timer->Init();
	g_pool->Init(PROJECTILE_MAX_NUM);

	CServerServiceRef service = std::make_shared<CServerService>(
		CNetAddress(ipAddress, 6767),
		std::make_shared<CIocpCore>(),
		[=]() { return std::make_shared<CGameSession>(); },
		100);

	ASSERT_CRASH(service->Start());

	for (int32 i = 0; i < 5; i++)
	{
		g_ThreadManager->Launch([&service]()
			{
				DoWorkerJob(service);
			});
	}


	// Main Thread
	g_Room->DoAsync(&CRoom::Update);	// Room 업데이트 등록
	DoWorkerJob(service);

	// 채팅 테스트 (동접 테스트)
	//using namespace std::chrono_literals;
	//while (true)
	//{
	//	Protocol::S_CHAT pkt;
	//	pkt.set_msg("HelloWorld");
	//	auto sendBuffer = ServerPacketHandler::MakeSendBuffer(pkt);

	//	g_SessionManager.Broadcast(sendBuffer);
	//	std::this_thread::sleep_for(0.1s);
	//}

	g_ThreadManager->Join();
}