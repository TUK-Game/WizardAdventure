#pragma once

class CRecvBuffer
{
	enum { BUFFER_COUNT = 10 };

public:
	CRecvBuffer(int32 bufferSize);
	~CRecvBuffer();

	void			Clean();
	bool			OnRead(int32 numOfBytes);
	bool			OnWrite(int32 numOfBytes);

	BYTE*			ReadPos() { return &m_Buffer[m_ReadPos]; }
	BYTE*			WritePos() { return &m_Buffer[m_WritePos]; }
	int32			DataSize() { return m_WritePos - m_ReadPos; }
	int32			FreeSize() { return m_Capacity - m_WritePos; }

private:
	int32				m_Capacity = 0;
	int32				m_BufferSize = 0;
	int32				m_ReadPos = 0;
	int32				m_WritePos = 0;
	std::vector<BYTE>	m_Buffer;
};

