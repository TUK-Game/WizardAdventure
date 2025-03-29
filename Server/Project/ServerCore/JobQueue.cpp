#include "pch.h"
#include "JobQueue.h"
#include "GlobalQueue.h"

void CJobQueue::Push(CJobRef job, bool pushOnly)
{
	const int32 prevCount = m_JobCount.fetch_add(1);
	m_Jobs.Push(job); // WRITE_LOCK

	// ù��° Job�� ���� �����尡 ������� ���
	if (prevCount == 0)
	{
		// �̹� �������� JobQueue�� ������ ����
		if (tl_CurrentJobQueue == nullptr && pushOnly == false)
		{
			Execute();
		}
		else
		{
			// ���� �ִ� �ٸ� �����尡 �����ϵ��� GlobalQueue�� �ѱ��
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

		// ���� �ϰ��� 0����� ����
		if (m_JobCount.fetch_sub(jobCount) == jobCount)
		{
			tl_CurrentJobQueue = nullptr;
			return;
		}

		const uint64 now = ::GetTickCount64();
		if (now >= tl_EndTickCount)
		{
			tl_CurrentJobQueue = nullptr;
			// ���� �ִ� �ٸ� �����尡 �����ϵ��� GlobalQueue�� �ѱ��
			g_GlobalQueue->Push(shared_from_this());
			break;
		}			
	}
}
