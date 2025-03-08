#include "pch.h"
#include "GlobalQueue.h"

CGlobalQueue::CGlobalQueue()
{

}

CGlobalQueue::~CGlobalQueue()
{

}

void CGlobalQueue::Push(CJobQueueRef jobQueue)
{
	m_JobQueues.Push(jobQueue);
}

CJobQueueRef CGlobalQueue::Pop()
{
	return m_JobQueues.Pop();
}
