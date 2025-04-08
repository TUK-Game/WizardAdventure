#pragma once

#include <mutex>
#include <atomic>

using BYTE = unsigned char;
using int8 = __int8;
using int16 = __int16;
using int32 = __int32;
using int64 = __int64;
using uint8 = unsigned __int8;
using uint16 = unsigned __int16;
using uint32 = unsigned __int32;
using uint64 = unsigned __int64;

#define USING_SHARED_PTR(name)	using name##Ref = std::shared_ptr<class name>;

USING_SHARED_PTR(CIocpCore);
USING_SHARED_PTR(CIocpObject);
USING_SHARED_PTR(CSession);
USING_SHARED_PTR(CPacketSession);
USING_SHARED_PTR(CListener);
USING_SHARED_PTR(CServerService);
USING_SHARED_PTR(CClientService);
USING_SHARED_PTR(CSendBuffer);
USING_SHARED_PTR(CJob);
USING_SHARED_PTR(CJobQueue);
USING_SHARED_PTR(CTimer);

#define size16(val)		static_cast<int16>(sizeof(val))
#define size32(val)		static_cast<int32>(sizeof(val))
#define len16(arr)		static_cast<int16>(sizeof(arr)/sizeof(arr[0]))
#define len32(arr)		static_cast<int32>(sizeof(arr)/sizeof(arr[0]))
