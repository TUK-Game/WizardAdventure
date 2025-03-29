#include "pch.h"
#include "JobTimer.h"
#include "JobQueue.h"

void CJobTimer::Reserve(uint64 tickAfter, std::weak_ptr<CJobQueue> owner, CJobRef job)
{
	const uint64 executeTick = ::GetTickCount64() + tickAfter;
	JobData* jobData = new JobData(owner, job);

	WRITE_LOCK;

	m_Items.push(TimerItem{ executeTick, jobData });
}

void CJobTimer::Distribute(uint64 now)
{
	// 한 번에 1 쓰레드만 통과
	if (m_bDistributing.exchange(true) == true)
		return;

	std::vector<TimerItem> items;

	{
		WRITE_LOCK;

		while (m_Items.empty() == false)
		{
			const TimerItem& timerItem = m_Items.top();
			if (now < timerItem.ExecuteTick)
				break;

			items.push_back(timerItem);
			m_Items.pop();
		}
	}

	for (TimerItem& item : items)
	{
		if (CJobQueueRef owner = item.JobData->Owner.lock())
			owner->Push(item.JobData->Job);

		delete item.JobData;		
	}

	// 끝났으면 풀어준다
	m_bDistributing.store(false);
}

void CJobTimer::Clear()
{
	WRITE_LOCK;

	while (m_Items.empty() == false)
	{
		const TimerItem& timerItem = m_Items.top();
		delete timerItem.JobData;
		m_Items.pop();
	}
}
