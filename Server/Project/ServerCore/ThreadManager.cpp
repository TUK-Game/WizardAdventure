#include "pch.h"
#include "ThreadManager.h"
#include "CoreTLS.h"
#include "Core.h"
#include "GlobalQueue.h"

CThreadManager::CThreadManager()
{
	// Main Thread
	InitTLS();
}

CThreadManager::~CThreadManager()
{
	Join();
}

void CThreadManager::Launch(std::function<void(void)> callback)
{
	std::lock_guard<std::mutex> guard(m_Lock);

	m_vecThread.push_back(std::thread([=]()
		{
			InitTLS();
			callback();
			DestroyTLS();
		}));
}

void CThreadManager::Join()
{
	for (std::thread& t : m_vecThread)
	{
		if (t.joinable())
			t.join();
	}
	m_vecThread.clear();
}

void CThreadManager::InitTLS()
{
	static std::atomic<uint32> SThreadId = 1;
	tl_ThreadId = SThreadId.fetch_add(1);
}

void CThreadManager::DestroyTLS()
{

}

void CThreadManager::DoGlobalQueueWork()
{
	while (true)
	{
		uint64 now = ::GetTickCount64();
		if (now > tl_EndTickCount)
			break;

		CJobQueueRef jobQueue = g_GlobalQueue->Pop();
		if (jobQueue == nullptr)
			break;

		jobQueue->Execute();
	}
}

void CThreadManager::DistributeReservedJobs()
{
	const uint64 now = ::GetTickCount64();

	g_JobTimer->Distribute(now);
}
