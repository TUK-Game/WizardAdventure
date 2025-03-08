#pragma once

class SendBufferChunk;

class CSendBuffer : std::enable_shared_from_this<CSendBuffer>
{
public:
	CSendBuffer(int32 bufferSize);
	~CSendBuffer();

	BYTE* Buffer() { return m_Buffer.data(); }
	int32 WriteSize() { return m_WriteSize; }
	int32 Capacity() { return static_cast<int32>(m_Buffer.size()); }

	void CopyData(void* data, int32 len);
	void Close(uint32 writeSize);

private:
	std::vector<BYTE>	m_Buffer;
	int32				m_WriteSize = 0;
};

