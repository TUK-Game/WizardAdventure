#include "pch.h"
#include "JobQueue.h"
#include "GlobalQueue.h"

void CJobQueue::Push(CJobRef job, bool pushOnly)
{
	const int32 prevCount = m_JobCount.fetch_add(1);
	m_Jobs.Push(job); // WRITE_LOCK

	// 첫번째 Job을 넣은 쓰레드가 실행까지 담당
	if (prevCount == 0)
	{
		// 이미 실행중인 JobQueue가 없으면 실행
		if (tl_CurrentJobQueue == nullptr && pushOnly == false)
		{
			Execute();
		}
		else
		{
			// 여유 있는 다른 쓰레드가 실행하도록 GlobalQueue에 넘긴다
			g_GlobalQueue->Push(shared_from_this());
		}
	}
}

void CJobQueue::Execute()
{
	tl_CurrentJobQueue = this;

	while (true)
	{
		std::vector<CJobRef> jobs;
		m_Jobs.PopAll(OUT jobs);

		const int32 jobCount = static_cast<int32>(jobs.size());
		for (int32 i = 0; i < jobCount; i++)
			jobs[i]->Execute();

		// 남은 일감이 0개라면 종료
		if (m_JobCount.fetch_sub(jobCount) == jobCount)
		{
			tl_CurrentJobQueue = nullptr;
			return;
		}

		const uint64 now = ::GetTickCount64();
		if (now >= tl_EndTickCount)
		{
			tl_CurrentJobQueue = nullptr;
			// 여유 있는 다른 쓰레드가 실행하도록 GlobalQueue에 넘긴다
			g_GlobalQueue->Push(shared_from_this());
			break;
		}			
	}
}
