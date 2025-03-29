#include "pch.h"
#include "BufferWriter.h"

CBufferWriter::CBufferWriter()
{

}

CBufferWriter::CBufferWriter(BYTE* buffer, uint32 size, uint32 pos)
	: m_Buffer(buffer), m_Size(size), m_Pos(pos)
{

}

CBufferWriter::~CBufferWriter()
{

}

bool CBufferWriter::Write(void* src, uint32 len)
{
	if (FreeSize() < len)
		return false;

	::memcpy(&m_Buffer[m_Pos], src, len);
	m_Pos += len;
	return true;
}