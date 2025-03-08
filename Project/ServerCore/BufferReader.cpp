#include "pch.h"
#include "BufferReader.h"

CBufferReader::CBufferReader()
{
}

CBufferReader::CBufferReader(BYTE* buffer, uint32 size, uint32 pos)
	: m_Buffer(buffer), m_Size(size), m_Pos(pos)
{

}

CBufferReader::~CBufferReader()
{

}

bool CBufferReader::Peek(void* dest, uint32 len)
{
	if (FreeSize() < len)
		return false;

	::memcpy(dest, &m_Buffer[m_Pos], len);
	return true;
}

bool CBufferReader::Read(void* dest, uint32 len)
{
	if (Peek(dest, len) == false)
		return false;

	m_Pos += len;
	return true;
}
