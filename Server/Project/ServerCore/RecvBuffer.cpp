#include "pch.h"
#include "RecvBuffer.h"

CRecvBuffer::CRecvBuffer(int32 bufferSize) : m_BufferSize(bufferSize)
{
	m_Capacity = bufferSize * BUFFER_COUNT;
	m_Buffer.resize(m_Capacity);
}

CRecvBuffer::~CRecvBuffer()
{
}

void CRecvBuffer::Clean()
{
	int32 dataSize = DataSize();
	if (dataSize == 0)
	{
		// 딱 마침 읽기+쓰기 커서가 동일한 위치라면, 둘 다 리셋.
		m_ReadPos = m_WritePos = 0;
	}
	else
	{
		// 여유 공간이 버퍼 1개 크기 미만이면, 데이터를 앞으로 땅긴다.
		if (FreeSize() < m_BufferSize)
		{
			::memcpy(&m_Buffer[0], &m_Buffer[m_ReadPos], dataSize);
			m_ReadPos = 0;
			m_WritePos = dataSize;
		}
	}
}

bool CRecvBuffer::OnRead(int32 numOfBytes)
{
	if (numOfBytes > DataSize())
		return false;

	m_ReadPos += numOfBytes;
	return true;
}

bool CRecvBuffer::OnWrite(int32 numOfBytes)
{
	if (numOfBytes > FreeSize())
		return false;

	m_WritePos += numOfBytes;
	return true;
}