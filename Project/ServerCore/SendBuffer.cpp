#include "pch.h"
#include "SendBuffer.h"

CSendBuffer::CSendBuffer(int32 bufferSize)
{
	m_Buffer.resize(bufferSize);
}

CSendBuffer::~CSendBuffer()
{
}

void CSendBuffer::CopyData(void* data, int32 len)
{
	assert(Capacity() >= len);
	::memcpy(m_Buffer.data(), data, len);
	m_WriteSize = len;
}

void CSendBuffer::Close(uint32 writeSize)
{
	m_WriteSize = writeSize;
}
