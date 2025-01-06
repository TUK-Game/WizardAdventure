#pragma once

template<typename T>
class CSingleton
{
private:
	static T* s_Inst;

public:
	static T* GetInst()
	{
		if (!s_Inst)
			s_Inst = new T;

		return s_Inst;
	}

	static void DestroyInst()
	{
		if (s_Inst)
		{
			delete s_Inst;
			s_Inst = nullptr;
		}
	}

protected:
	CSingleton()
	{
		typedef void(*FUNC)(void);
		atexit((FUNC)&CSingleton::DestroyInst);
	}

	~CSingleton() {}
};


template<typename T>
T* CSingleton<T>::s_Inst = nullptr;