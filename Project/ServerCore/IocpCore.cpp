#include "pch.h"
#include "IocpCore.h"
#include "IocpEvent.h"

CIocpCore::CIocpCore()
{
	m_IocpHandle = ::CreateIoCompletionPort(INVALID_HANDLE_VALUE, 0, 0, 0);
	ASSERT_CRASH(m_IocpHandle != INVALID_HANDLE_VALUE);
}

CIocpCore::~CIocpCore()
{
	::CloseHandle(m_IocpHandle);
}

bool CIocpCore::Register(CIocpObjectRef iocpObject)
{
	return ::CreateIoCompletionPort(iocpObject->GetHandle(), m_IocpHandle, /*key*/0, 0);
}

bool CIocpCore::Dispatch(uint32 timeoutMs)
{
	DWORD numOfBytes = 0;
	ULONG_PTR key = 0;	
	CIocpEvent* iocpEvent = nullptr;

	if (::GetQueuedCompletionStatus(m_IocpHandle, OUT &numOfBytes, OUT &key, OUT reinterpret_cast<LPOVERLAPPED*>(&iocpEvent), timeoutMs))
	{
		CIocpObjectRef iocpObject = iocpEvent->m_Owner;
		iocpObject->Dispatch(iocpEvent, numOfBytes);
	}
	else
	{
		int32 errCode = ::WSAGetLastError();
		switch (errCode)
		{
		case WAIT_TIMEOUT:
			return false;
		default:
			// TODO : ·Î±× Âï±â
			CIocpObjectRef iocpObject = iocpEvent->m_Owner;
			iocpObject->Dispatch(iocpEvent, numOfBytes);
			break;
		}
	}

	return true;
}
