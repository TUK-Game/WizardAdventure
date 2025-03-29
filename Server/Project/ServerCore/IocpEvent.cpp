#include "pch.h"
#include "IocpEvent.h"

CIocpEvent::CIocpEvent(EEventType type) : m_EventType(type)
{
	Init();
}

void CIocpEvent::Init()
{
	OVERLAPPED::hEvent = 0;
	OVERLAPPED::Internal = 0;
	OVERLAPPED::InternalHigh = 0;
	OVERLAPPED::Offset = 0;
	OVERLAPPED::OffsetHigh = 0;
}
