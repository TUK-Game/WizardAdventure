#include "pch.h"
#include "Util.h"

std::wstring CUtil::GetMyIPv4Address()
{
    WSADATA wsaData;
    char hostname[256];
    std::wstring ipAddress = L"Not found";

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::wcerr << L"WSAStartup failed.\n";
        return ipAddress;
    }

    if (gethostname(hostname, sizeof(hostname)) == SOCKET_ERROR) {
        std::wcerr << L"Error getting hostname.\n";
        WSACleanup();
        return ipAddress;
    }

    addrinfo hints = {}, * info = nullptr;
    hints.ai_family = AF_INET; // IPv4 only

    if (getaddrinfo(hostname, nullptr, &hints, &info) != 0) {
        std::wcerr << L"getaddrinfo failed.\n";
        WSACleanup();
        return ipAddress;
    }

    for (addrinfo* ptr = info; ptr != nullptr; ptr = ptr->ai_next) {
        sockaddr_in* sockaddr_ipv4 = reinterpret_cast<sockaddr_in*>(ptr->ai_addr);
        char ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &(sockaddr_ipv4->sin_addr), ip, INET_ADDRSTRLEN);

        // char → wstring 변환
        int size_needed = MultiByteToWideChar(CP_UTF8, 0, ip, -1, nullptr, 0);
        wchar_t* wide_ip = new wchar_t[size_needed];
        MultiByteToWideChar(CP_UTF8, 0, ip, -1, wide_ip, size_needed);
        ipAddress = wide_ip;
        delete[] wide_ip;

        break; // 첫 번째 IP만 사용
    }

    freeaddrinfo(info);
    WSACleanup();
    return ipAddress;
}
