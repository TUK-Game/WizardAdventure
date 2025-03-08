#include "pch.h"
#include "NetAddress.h"

CNetAddress::CNetAddress(SOCKADDR_IN sockAddr) : m_SockAddr(sockAddr)
{
}

CNetAddress::CNetAddress(std::wstring ip, uint16 port)
{
	::memset(&m_SockAddr, 0, sizeof(m_SockAddr));
	m_SockAddr.sin_family = AF_INET;
	m_SockAddr.sin_addr = Ip2Address(ip.c_str());
	m_SockAddr.sin_port = ::htons(port);
}

std::wstring CNetAddress::GetIpAddress()
{
	WCHAR buffer[100];
	::InetNtopW(AF_INET, &m_SockAddr.sin_addr, buffer, len32(buffer));
	return std::wstring(buffer);
}

IN_ADDR CNetAddress::Ip2Address(const WCHAR* ip)
{
	IN_ADDR address;
	::InetPtonW(AF_INET, ip, &address);
	return address;
}
