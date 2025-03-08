#pragma once

class CGlobalQueue
{
public:
	CGlobalQueue();
	~CGlobalQueue();

	void					Push(CJobQueueRef jobQueue);
	CJobQueueRef				Pop();

private:
	CLockQueue<CJobQueueRef> m_JobQueues;
};

