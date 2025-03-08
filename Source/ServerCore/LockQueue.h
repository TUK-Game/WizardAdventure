#pragma once

template<typename T>
class CLockQueue
{
public:
	void Push(T data)
	{
		WRITE_LOCK;
		m_DataQueue.push(data);
	}

	T Pop()
	{
		WRITE_LOCK;
		return PopNoLock();
	}

	T PopNoLock()
	{
		if (m_DataQueue.empty())
			return T();

		T ret = m_DataQueue.front();
		m_DataQueue.pop();
		return ret;
	}

	void PopAll(OUT std::vector<T>& vecData)
	{
		WRITE_LOCK;
		while (T data = PopNoLock())
			vecData.push_back(data);
	}

	void Clear()
	{
		WRITE_LOCK;
		m_DataQueue = std::queue<T>();
	}

private:
	USE_LOCK;
	std::queue<T> m_DataQueue;
};