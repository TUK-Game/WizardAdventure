#pragma once

class CIocpObject : public std::enable_shared_from_this<CIocpObject>
{
public:
	virtual HANDLE GetHandle() = 0;
	virtual void Dispatch(class CIocpEvent* iocpEvent, int32 numOfBytes = 0) = 0;
};



class CIocpCore
{
public:
	CIocpCore();
	~CIocpCore();

	HANDLE		GetHandle() { return m_IocpHandle; }

	bool		Register(CIocpObjectRef iocpObject);
	bool		Dispatch(uint32 timeoutMs = INFINITE);

private:
	HANDLE		m_IocpHandle;
};