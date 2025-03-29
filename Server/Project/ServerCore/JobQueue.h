#pragma once
#include "Job.h"
#include "LockQueue.h"
#include "JobTimer.h"

class CJobQueue : public std::enable_shared_from_this<CJobQueue>
{
public:
	void DoAsync(std::function<void()>&& callback)
	{
		Push(std::make_shared<CJob>(std::move(callback)));
	}

	template<typename T, typename Ret, typename... Args>
	void DoAsync(Ret(T::* memFunc)(Args...), Args... args)
	{
		std::shared_ptr<T> owner = std::static_pointer_cast<T>(shared_from_this());
		Push(std::make_shared<CJob>(owner, memFunc, std::forward<Args>(args)...));
	}

	void DoTimer(uint64 tickAfter, std::function<void()>&& callback)
	{
		CJobRef job = std::make_shared<CJob>(std::move(callback));
		g_JobTimer->Reserve(tickAfter, shared_from_this(), job);
	}

	template<typename T, typename Ret, typename... Args>
	void DoTimer(uint64 tickAfter, Ret(T::* memFunc)(Args...), Args... args)
	{
		std::shared_ptr<T> owner = std::static_pointer_cast<T>(shared_from_this());
		CJobRef job = std::make_shared<CJob>(owner, memFunc, std::forward<Args>(args)...);
		g_JobTimer->Reserve(tickAfter, shared_from_this(), job);
	}

	void					ClearJobs() { m_Jobs.Clear(); }

public:
	void					Push(CJobRef job, bool pushOnly = false);
	void					Execute();

protected:
	CLockQueue<CJobRef>		m_Jobs;
	std::atomic<int32>		m_JobCount = 0;
};

