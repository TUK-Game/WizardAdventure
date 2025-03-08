#pragma once

struct JobData
{
	JobData(std::weak_ptr<CJobQueue> owner, CJobRef job) : Owner(owner), Job(job)
	{

	}

	std::weak_ptr<CJobQueue>	Owner;
	CJobRef						Job;
};

struct TimerItem
{
	bool operator<(const TimerItem& other) const
	{
		return ExecuteTick > other.ExecuteTick;
	}

	uint64 ExecuteTick = 0;
	JobData* JobData = nullptr;
};



class CJobTimer
{
public:
	void			Reserve(uint64 tickAfter, std::weak_ptr<CJobQueue> owner, CJobRef job);
	void			Distribute(uint64 now);
	void			Clear();

private:
	USE_LOCK;
	std::priority_queue<TimerItem>	m_Items;
	std::atomic<bool>				m_bDistributing = false;
};

